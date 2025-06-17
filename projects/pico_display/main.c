#include <stdio.h>
#include "pico/stdlib.h"
#include "st7789.h"

const struct st7789_config lcd_config = {
    // .spi      = PICO_DEFAULT_SPI_INSTANCE,
    .spi      = spi0 ,
    .gpio_din = PICO_DEFAULT_SPI_TX_PIN,
    .gpio_clk = PICO_DEFAULT_SPI_SCK_PIN,
    .gpio_cs  = PICO_DEFAULT_SPI_CSN_PIN,
    .gpio_dc  = 20,
    .gpio_rst = 21,
    .gpio_bl  = 22,
};

const int lcd_width = 240;
const int lcd_height = 280;

int main() {
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    stdio_init_all();
    printf("Hello, Raspberry Pi Pico!\n");
    // initialize the lcd
    st7789_init(&lcd_config, lcd_width, lcd_height);
	
    while (1) {
        printf("Blinking LED on pin %d\n", LED_PIN);
        gpio_put(LED_PIN, 1);
        st7789_fill(0x0000);
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
		st7789_fill(0xffff);
        sleep_ms(500);
    }
}
