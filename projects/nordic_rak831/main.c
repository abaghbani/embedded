#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrfx_twim.h"
#include "boards.h"

#include "app_util_platform.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "spi_driver.h"

#define pin_spi_clk				29
#define pin_spi_mosi			4
#define pin_spi_miso			28
#define pin_spi_cs_n			3
#define pin_spi_reset			30

void reset_spi()
{
	nrf_gpio_cfg_output(pin_spi_reset);
	nrf_gpio_pin_clear(pin_spi_reset);
	nrf_delay_ms(1);
	nrf_gpio_pin_set(pin_spi_reset);
	nrf_delay_ms(1);
	nrf_gpio_pin_clear(pin_spi_reset);
}

int main(void)
{
	uint8_t tx_buf[20];
	uint8_t rx_buf[20];
	
	// sdk init
	bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS);
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	reset_spi();
	
	// spi init
	Spi_t spi;
	spi.spi = NRF_SPI0;
	spi.clk_pin	 = pin_spi_clk;
	spi.mosi_pin = pin_spi_mosi;
	spi.miso_pin = pin_spi_miso;
	spi.cs_pin	 = pin_spi_cs_n;
	spi.mode = NRF_SPI_MODE_0;
	spi.int_enabled = false;
	if(spi_init(&spi) == 0)
		NRF_LOG_INFO("Error: spi initialization is failed.");
	
	// start read/write to spi
	spi_xfer(&spi, 0x44);
	
	tx_buf[0] = 0x80;
	tx_buf[1] = 0x81;
	spi_xfer_buf(&spi, tx_buf, 2, rx_buf);
	
	while(1)
	{
		spi_xfer_buf(&spi, tx_buf, 17, rx_buf);
		NRF_LOG_INFO("thermal sensor status = 0x%x", rx_buf[16]);
		
		
		NRF_LOG_INFO("loop is running...");
		NRF_LOG_FLUSH();
		bsp_board_led_invert(BSP_BOARD_LED_0);
		nrf_delay_ms(2000);
	}
}
