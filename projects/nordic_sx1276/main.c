#include <stdio.h>
#include <string.h>
#include "board.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "spi_driver.h"
#include "sx1276.h"

#include "app_util_platform.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

void bsp_initialization(void)
{
	nrf_gpio_cfg_output(BSP_LED_0);
	nrf_gpio_cfg_output(BSP_LED_1);
	nrf_gpio_pin_clear(BSP_LED_0);
	nrf_gpio_pin_clear(BSP_LED_1);
	nrf_gpio_cfg_input(BSP_BUTTON_0, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(BSP_BUTTON_1, NRF_GPIO_PIN_NOPULL);
	
	// enable GPS module
	nrf_gpio_cfg_output(GPS_PWR_PIN);
	nrf_gpio_pin_set(GPS_PWR_PIN);
	
	nrf_gpio_cfg_input(GPS_TIMEPULSE_PIN, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(LIS3DH_INT1_PIN, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_input(LIS3DH_INT2_PIN, NRF_GPIO_PIN_NOPULL);
}

void spi_initialization(Spi_t *spi)
{
	if (spi_init(spi))
	{
		NRF_LOG_INFO("SPI initialization is done, SPI started...");
	}
}

void sx1276_initialization(SX1276_t *chipAccess)
{
	uint32_t freq = sx1276_init(chipAccess);
	NRF_LOG_INFO("chip version = %x", sx1276_read(chipAccess, REG_LR_VERSION));
	NRF_LOG_INFO("freq = " NRF_LOG_FLOAT_MARKER " MHz", NRF_LOG_FLOAT((double)(freq)*FREQ_STEP / 1000000));
	NRF_LOG_INFO("sx1276 opmode = %x", sx1276_read(chipAccess, REG_LR_OPMODE));
}

int main(void)
{
	bsp_initialization();

	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	NRF_LOG_INFO("log is initialized, started:");

	// spi initialization
	Spi_t spi_init;
	spi_init.spi = NRF_SPI0;
	spi_init.cs_pin = RADIO_NSS_PIN;
	spi_init.clk_pin = RADIO_SCK_PIN;
	spi_init.mosi_pin = RADIO_MOSI_PIN;
	spi_init.miso_pin = RADIO_MISO_PIN;
	spi_init.int_enabled = false;
	spi_initialization(&spi_init);

	// sx1276 initialization
	SX1276_t sx1276;
	sx1276.spi = &spi_init;
	sx1276.dio_0_pin = RADIO_DIO0_PIN;
	sx1276.dio_1_pin = RADIO_DIO1_PIN;
	sx1276.dio_2_pin = RADIO_DIO2_PIN;
	sx1276.dio_3_pin = RADIO_DIO3_PIN;
	sx1276.reset_pin = RADIO_DIO3_PIN;
	sx1276.tcxo_pin = RADIO_TCXO_PIN;
	sx1276.ctx_pin = RADIO_RF_CTX_PIN;
	sx1276.cps_pin = RADIO_RF_CPS_PIN;
	sx1276_initialization(&sx1276);

	uint8_t txbuf[] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a};
	sx1276_transmit(&sx1276, txbuf);
	NRF_LOG_INFO("transmit is done.");

	uint8_t bandwidth_index = 0;
	uint8_t sf_index = 0;

	while (1)
	{
		if(nrf_gpio_pin_read(RADIO_DIO0_PIN) == 1)
		{
			NRF_LOG_INFO("transmit is done.");
			NRF_LOG_INFO("sx1276 opmode = %x", sx1276_read(&sx1276, REG_LR_OPMODE));
			sx1276_transmit(&sx1276, txbuf);
		}

		if (nrf_gpio_pin_read(BSP_BUTTON_0) == 0)
		{
			if(++bandwidth_index>9)
			{
				bandwidth_index = 0;
			}

			switch(bandwidth_index)
			{
				case 0:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_7_81_KHZ; break;
				case 1:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_10_41_KHZ; break;
				case 2:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_15_62_KHZ; break;
				case 3:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_20_83_KHZ; break;
				case 4:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_31_25_KHZ; break;
				case 5:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_41_66_KHZ; break;
				case 6:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_62_50_KHZ; break;
				case 7:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_125_KHZ; break;
				case 8:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_250_KHZ; break;
				case 9:		sx1276.modem.band_width			= RFLR_MODEMCONFIG1_BW_500_KHZ; break;
			}
			sx1276_modem_setting(&sx1276);
			NRF_LOG_INFO("key_0 is pressed, band_width_index = %x", bandwidth_index);
		}

		if (nrf_gpio_pin_read(BSP_BUTTON_1) == 0)
		{
			if(++sf_index>6)
			{
				sf_index = 0;
			}

			switch(sf_index)
			{
				case 0:		sx1276.modem.spread_factor		= RFLR_MODEMCONFIG2_SF_6; break;
				case 1:		sx1276.modem.spread_factor		= RFLR_MODEMCONFIG2_SF_7; break;
				case 2:		sx1276.modem.spread_factor		= RFLR_MODEMCONFIG2_SF_8; break;
				case 3:		sx1276.modem.spread_factor		= RFLR_MODEMCONFIG2_SF_9; break;
				case 4:		sx1276.modem.spread_factor		= RFLR_MODEMCONFIG2_SF_10; break;
				case 5:		sx1276.modem.spread_factor		= RFLR_MODEMCONFIG2_SF_11; break;
				case 6:		sx1276.modem.spread_factor		= RFLR_MODEMCONFIG2_SF_12; break;
			}
			sx1276_modem_setting(&sx1276);
			NRF_LOG_INFO("key_1 is pressed, spread_factor_index = %x", sf_index);
		}

		NRF_LOG_FLUSH();
		nrf_gpio_pin_toggle(BSP_LED_0);
		nrf_gpio_pin_toggle(BSP_LED_1);
		nrf_delay_ms(500);
	}
}
