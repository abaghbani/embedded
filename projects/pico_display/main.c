#include <stdio.h>
#include "pico/stdlib.h"
#include "st7789.h"
#include "itap.h"

const struct st7789_config lcd_config = {
    .spi      = spi0 ,
    .gpio_din = ITAP_PROBE_SPI0_TX,
    .gpio_clk = ITAP_PROBE_SPI0_SCK,
    .gpio_cs  = ITAP_PROBE_SPI0_CSN,
    .gpio_dc  = ITAP_PROBE_SPI0_RX,
    .gpio_rst = ITAP_PROBE_RST_DRAIN,
    .gpio_bl  = ITAP_PROBE_DET_DRAIN,
};

const int lcd_width = 240;
const int lcd_height = 280;

int main() {
    // const uint LED_PIN = 25;
    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);

    stdio_init_all();
    printf("Hello, Raspberry Pi Pico!\n");
    // initialize the lcd
    st7789_init(&lcd_config, lcd_width, lcd_height);
	itap_led_init();
	itap_led_set(200, 0, 0);
	itap_vref_init();
	itap_sensing_init();
	
    while (1) {
        printf("Blinking LED on color %d\n", itap_sensing_mv_get(ITAP_PROBE_ADC_VIO_CH));
        itap_led_set(200, 0, 0);
        st7789_fill(0x0000);
        sleep_ms(500);
        printf("Blinking LED on color %d\n", itap_sensing_mv_get(ITAP_PROBE_ADC_VTGT_CH));
        itap_led_set(0, 200, 0);
		st7789_fill(0xffff);
        sleep_ms(500);
    }
}
