#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

void hal_adc_init(void);
uint16_t hal_adc_read_channel(uint8_t channel);

#endif /* HAL_ADC_H */