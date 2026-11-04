#include "hal_pwm.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

/*
 * Khởi tạo PWM TIM2_CH1 (Chân PA0) điều khiển Servo 5V
 * - Tần số Clock: 72 MHz
 * - Prescaler = 71  -> Tần số đếm 1 MHz (1 tick = 1 us)
 * - Period = 19999  -> Chu kỳ PWM = 20000 us = 20 ms (50 Hz)
 */
void hal_pwm_init(void) {
    /* 1. Bật xung nhịp Clock cho GPIOA và TIM2 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* 2. Cấu hình chân PA0 làm ngõ ra AF_PP (Alternate Function Push-Pull) */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 3. Cấu hình Time Base cho Timer 2 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;           /* Tần số đếm 1 MHz */
    TIM_TimeBaseStructure.TIM_Period = 19999;           /* Chu kỳ 20 ms (50 Hz) */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* 4. Cấu hình Kênh 1 (TIM2_CH1) ở chế độ PWM Mode 1 */
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500;                /* Vị trí góc 90 độ ban đầu (1.5 ms) */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    /* 5. Cho phép Preload và Bật đếm Timer 2 */
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void hal_pwm_set_servo_angle(uint8_t angle) {
    if (angle > 180) {
        angle = 180;
    }
    
    /* Quy đổi góc (0 - 180 độ) sang thời lượng xung Pulse (1000 us - 2000 us) */
    uint16_t pulse = 1000 + ((uint32_t)angle * 1000 / 180);
    
    /* Gán giá trị Compare cho Kênh 1 của TIM2 */
    TIM_SetCompare1(TIM2, pulse);
}