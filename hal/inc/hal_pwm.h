#ifndef HAL_PWM_H
#define HAL_PWM_H

#include <stdint.h>

void hal_pwm_init(void);
void hal_pwm_set_servo_angle(uint8_t angle);

#endif /* HAL_PWM_H */