/**
 * @file knock_config.h
 *
 * uaBrain: two Hellen knock modules feeding the mega-mcu100-f7 module
 * knock 1: J5-7 -> knock module -> IN_KNOCK (PA2)
 * knock 2: J14-7 -> knock module -> IN_O2S2 (PA1)
 */

#pragma once

// Knock is on ADC3
#define KNOCK_ADC ADCD3

// knock 1 - pin PA2
#define KNOCK_ADC_CH1 ADC_CHANNEL_IN2
#define KNOCK_PIN_CH1 Gpio::A2

// knock 2 - pin PA1
#define KNOCK_HAS_CH2 true
#define KNOCK_ADC_CH2 ADC_CHANNEL_IN1
#define KNOCK_PIN_CH2 Gpio::A1

// Sample rate & time - depends on the exact MCU
#define KNOCK_SAMPLE_TIME ADC_SAMPLE_84
#define KNOCK_SAMPLE_RATE (STM32_PCLK2 / (4 * (84 + 12)))
