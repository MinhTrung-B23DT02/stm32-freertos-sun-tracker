#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "app_ipc.h"
#include "hal_pwm.h"
#include "task_tracking.h"

#define LDR_THRESHOLD    200   /* Ngưỡng sai lệch ánh sáng tối thiểu */
#define STEP_ANGLE       2     /* Bước điều chỉnh mỗi lần quay (độ) */

static TaskHandle_t xTrackingTaskHandle = NULL;
static uint8_t current_angle = 90; /* Khởi tạo góc 90 độ */

static void prvTrackingTask(void *pvParameters) {
    LdrData_t ldr_data;
    SystemStatus_t sys_status;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(50); /* Chu kỳ 50ms */

    hal_pwm_init();
    hal_pwm_set_servo_angle(current_angle);

    for (;;) {
        /* Nhận dữ liệu LDR từ Queue qLDR do TaskSensor gửi sang */
        if (xQueueReceive(qLDR, &ldr_data, portMAX_DELAY) == pdTRUE) {
            
            if (ldr_data.ldr_left > (ldr_data.ldr_right + LDR_THRESHOLD)) {
                if (current_angle + STEP_ANGLE <= 180) {
                    current_angle += STEP_ANGLE;
                }
            } 
            else if (ldr_data.ldr_right > (ldr_data.ldr_left + LDR_THRESHOLD)) {
                if (current_angle >= STEP_ANGLE) {
                    current_angle -= STEP_ANGLE;
                }
            }

            /* Cập nhật xung PWM điều khiển góc quay Servo */
            hal_pwm_set_servo_angle(current_angle);

            /* Gửi trạng thái góc sang TaskDisplay */
            sys_status.servo_angle = current_angle;
            xQueueSend(qStatus, &sys_status, 0);
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

BaseType_t task_tracking_create(void) {
    return xTaskCreate(
        prvTrackingTask,
        "TRACKING_TASK",
        configMINIMAL_STACK_SIZE + 128,
        NULL,
        tskIDLE_PRIORITY + 2,
        &xTrackingTaskHandle
    );
}