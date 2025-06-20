#include <string.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/adc.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include "itap.h"

void itap_led_set(int r, int g, int b)
{
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    pwm_set_chan_level(0, 0, r);
    pwm_set_chan_level(0, 1, g);
    pwm_set_chan_level(1, 0, b);
}

void itap_led_init(void)
{
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 10);
    pwm_config_set_wrap(&config, 255);
    pwm_config_set_output_polarity(&config, true, true);
    pwm_init(0, &config, true);
    pwm_config_set_output_polarity(&config, true, false);
    pwm_init(1, &config, true);

	gpio_set_function(ITAP_PROBE_LED_R, GPIO_FUNC_PWM);
	gpio_set_function(ITAP_PROBE_LED_G, GPIO_FUNC_PWM);
	gpio_set_function(ITAP_PROBE_LED_B, GPIO_FUNC_PWM);
}

void itap_vref_init(void)
{
    gpio_set_function(ITAP_PROBE_VIO_ADJ_PWM, GPIO_FUNC_PWM);
    gpio_set_pulls(ITAP_PROBE_VIO_ADJ_PWM, 0, 0);
    uint num = pwm_gpio_to_slice_num(ITAP_PROBE_VIO_ADJ_PWM);
    uint chan = pwm_gpio_to_channel(ITAP_PROBE_VIO_ADJ_PWM);
    pwm_set_chan_level(num, chan, 22);

    gpio_init(ITAP_PROBE_VTGT_EN);
    gpio_set_dir(ITAP_PROBE_VTGT_EN, GPIO_OUT);
    gpio_put(ITAP_PROBE_VTGT_EN, 1);
	
    gpio_init(ITAP_PROBE_TMS_PU_EN);
    gpio_set_dir(ITAP_PROBE_TMS_PU_EN, GPIO_OUT);
    gpio_put(ITAP_PROBE_TMS_PU_EN, 0);
	
    gpio_init(ITAP_PROBE_TCK_PU_EN);
    gpio_set_dir(ITAP_PROBE_TCK_PU_EN, GPIO_OUT);
    gpio_put(ITAP_PROBE_TCK_PU_EN, 0);

    gpio_init(ITAP_PROBE_TDO_O);
    gpio_set_dir(ITAP_PROBE_TDO_O, GPIO_OUT);
    gpio_put(ITAP_PROBE_TDO_O, 1);

    gpio_init(ITAP_PROBE_TDI_O);
    gpio_set_dir(ITAP_PROBE_TDI_O, GPIO_OUT);
    gpio_put(ITAP_PROBE_TDI_O, 1);

    gpio_init(ITAP_PROBE_TCK_O);
    gpio_set_dir(ITAP_PROBE_TCK_O, GPIO_OUT);
    gpio_put(ITAP_PROBE_TCK_O, 1);

    gpio_init(ITAP_PROBE_TMS_O);
    gpio_set_dir(ITAP_PROBE_TMS_O, GPIO_OUT);
    gpio_put(ITAP_PROBE_TMS_O, 1);
	
    gpio_init(ITAP_PROBE_TRN_OE_n);
    gpio_set_dir(ITAP_PROBE_TRN_OE_n, GPIO_OUT);
    gpio_put(ITAP_PROBE_TRN_OE_n, 0);
}

void itap_sensing_init(void)
{
    adc_init();
    adc_gpio_init(ITAP_PROBE_ADC_TMS);
    adc_gpio_init(ITAP_PROBE_ADC_TDO);
    adc_gpio_init(ITAP_PROBE_ADC_VIO);
    adc_gpio_init(ITAP_PROBE_ADC_VTGT);
    gpio_set_pulls(ITAP_PROBE_ADC_TMS, 0, 0);
    gpio_set_pulls(ITAP_PROBE_ADC_TDO, 0, 0);
    gpio_set_pulls(ITAP_PROBE_ADC_VIO, 0, 0);
    gpio_set_pulls(ITAP_PROBE_ADC_VTGT, 0, 0);
}

int itap_sensing_mv_get(int channel)
{
    adc_select_input(channel);
    busy_wait_us_32(100);
    uint16_t raw = adc_read();
    return (3300 * raw) >> 12;
}
