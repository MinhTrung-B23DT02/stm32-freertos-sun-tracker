#ifndef APP_IPC_H
#define APP_IPC_H

#include "FreeRTOS.h"
#include "queue.h"
#include <stdint.h>

/* Cấu trúc dữ liệu cảm biến LDR (TaskSensor -> TaskTracking) */
typedef struct {
    uint16_t ldr_left;   /* Giá trị ADC LDR Trái */
    uint16_t ldr_right;  /* Giá trị ADC LDR Phải */
} LdrData_t;

/* Cấu trúc dữ liệu hệ thống & điện áp (TaskTracking -> TaskDisplay) */
typedef struct {
    uint8_t servo_angle; /* Góc quay Servo hiện tại (0 - 180 deg) */
    float voltage;       /* Điện áp đo từ INA219 (V) */
    float current;       /* Dòng điện đo từ INA219 (mA) */
    float power;         /* Công suất P = U x I (mW) */
} SystemStatus_t;

/* Khai báo Queue Handle dùng chung giữa các Task */
extern QueueHandle_t qLDR;
extern QueueHandle_t qStatus;

/* Hàm khởi tạo các đối tượng IPC (sẽ gọi trong main.c) */
void app_ipc_init(void);

#endif /* APP_IPC_H */