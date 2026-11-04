#include "stm32f10x.h"
#include "hal_pwm.h"

void delay_ms_simple(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 7200; i++) {
        __NOP();
    }
}

int main(void) {
    hal_pwm_init();

    while (1) {
        /* Quay về góc 0 độ */
        hal_pwm_set_servo_angle(0);
        delay_ms_simple(1000);

        /* Quay về góc 90 độ */
        hal_pwm_set_servo_angle(90);
        delay_ms_simple(1000);

        /* Quay về góc 180 độ */
        hal_pwm_set_servo_angle(180);
        delay_ms_simple(1000);
    }
}