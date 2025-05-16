#include <string.h>
#include "nrfx.h"
#include "sx1276.h"
#include "nrf_gpio.h"
#include "spi_driver.h"
#include "nrf_delay.h"

uint32_t sx1276_init(SX1276_t * chipAccess)
{
	sx1276_ios_init(chipAccess);
	sx1276_reset(chipAccess);
	
	sx1276_write(chipAccess, REG_LR_OPMODE, RFLR_OPMODE_SLEEP);
	sx1276_write(chipAccess, REG_LR_OPMODE, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_FREQMODE_ACCESS_HF | RFLR_OPMODE_SLEEP);
	sx1276_write(chipAccess, REG_LR_OPMODE, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_FREQMODE_ACCESS_HF | RFLR_OPMODE_STANDBY);
	
	// d3: CadDone d2: FhssChangeChannel d1: RxTimeout d0: TxDone
	sx1276_write(chipAccess, REG_LR_DIOMAPPING1, 0x40);
	sx1276_setFrequency(chipAccess, 868100000);
	nrf_delay_ms(6);
	sx1276_write(chipAccess, REG_LR_PACONFIG, 0x80);
	sx1276_write(chipAccess, REG_LR_PARAMP, 0x09);
	sx1276_write(chipAccess, REG_LR_OCP, 0x2b);
	sx1276_write(chipAccess, REG_LR_LNA, 0x20);
	
	// lora page regs
	sx1276_write(chipAccess, REG_LR_FIFOADDRPTR, RFLR_FIFOADDRPTR);
	sx1276_write(chipAccess, REG_LR_FIFOTXBASEADDR, RFLR_FIFOTXBASEADDR);
	sx1276_write(chipAccess, REG_LR_FIFORXBASEADDR, RFLR_FIFORXBASEADDR);
	sx1276_write(chipAccess, REG_LR_IRQFLAGSMASK, 0);
	sx1276_write(chipAccess, REG_LR_IRQFLAGS, 0xff);
	
	// modem configuration
	chipAccess->modem.band_width			= RFLR_MODEMCONFIG1_BW_125_KHZ;
	chipAccess->modem.coding_rate			= RFLR_MODEMCONFIG1_CODINGRATE_4_5;
	chipAccess->modem.implicit_enable		= RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF;
	chipAccess->modem.spread_factor			= RFLR_MODEMCONFIG2_SF_7;
	chipAccess->modem.tx_continous_enable	= RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_OFF;
	chipAccess->modem.rx_symb_timeout		= 0x64;
	chipAccess->modem.preamble_number		= 0x0008;
	sx1276_modem_setting(chipAccess);
	
	return sx1276_getFrequency(chipAccess);
}

void sx1276_modem_setting(SX1276_t * chipAccess)
{
	sx1276_write(chipAccess, REG_LR_MODEMCONFIG1, chipAccess->modem.band_width | chipAccess->modem.coding_rate | chipAccess->modem.implicit_enable);
	sx1276_write(chipAccess, REG_LR_MODEMCONFIG2, chipAccess->modem.spread_factor | chipAccess->modem.tx_continous_enable);
	sx1276_write(chipAccess, REG_LR_SYMBTIMEOUTLSB, chipAccess->modem.rx_symb_timeout);
	sx1276_write(chipAccess, REG_LR_PREAMBLEMSB, (uint8_t)(chipAccess->modem.preamble_number >> 8));
	sx1276_write(chipAccess, REG_LR_PREAMBLELSB, (uint8_t)(chipAccess->modem.preamble_number & 0xff));
	
	// all the fix value of modem setting (according to semtech datasheet)
	sx1276_write(chipAccess, REG_LR_PAYLOADLENGTH, 0x01);
	sx1276_write(chipAccess, REG_LR_PAYLOADMAXLENGTH, 0xff);
	sx1276_write(chipAccess, REG_LR_HOPPERIOD, RFLR_HOPPERIOD_FREQFOPPINGPERIOD);
	sx1276_write(chipAccess, REG_LR_MODEMCONFIG3, RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_OFF | RFLR_MODEMCONFIG3_AGCAUTO_OFF);
	sx1276_write(chipAccess, REG_LR_TEST2F, 0x20);
	sx1276_write(chipAccess, REG_LR_TEST30, 0);
	sx1276_write(chipAccess, REG_LR_DETECTOPTIMIZE, RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12);
	sx1276_write(chipAccess, REG_LR_INVERTIQ, 0);
	sx1276_write(chipAccess, REG_LR_DETECTIONTHRESHOLD, 0x0A);
	sx1276_write(chipAccess, REG_LR_SYNCWORD, 0x12);
	sx1276_write(chipAccess, REG_LR_TEST3A, 0x20);
	sx1276_write(chipAccess, REG_LR_INVERTIQ2, 0x1D);
}

void sx1276_transmit(SX1276_t * chipAccess, uint8_t * txData)
{
	chipAccess->rf_setting.rf_switch_state = RF_TX_RUNNING;
	sx1276_rfSwitch(chipAccess);
	// sx1276_write(chipAccess, REG_LR_PAYLOADLENGTH, 0x0a);
	sx1276_burstWrite(chipAccess, REG_LR_FIFO, txData, 10);
	sx1276_write(chipAccess, REG_LR_OPMODE, (sx1276_read(chipAccess, REG_LR_OPMODE) & RFLR_OPMODE_MASK) | RFLR_OPMODE_TRANSMITTER);
}

void sx1276_write(SX1276_t * chipAccess, uint8_t addr, uint8_t data)
{
	uint8_t tx_buf[2] = {AddWriteFlag(addr), data};
	uint8_t rx_buf[2];
	spi_xfer_buf(chipAccess->spi, tx_buf, 2, rx_buf);
}

uint8_t sx1276_read(SX1276_t * chipAccess, uint8_t addr)
{
	uint8_t tx_buf[2] = {AddReadFlag(addr), 0x00};
	uint8_t rx_buf[2];
	spi_xfer_buf(chipAccess->spi, tx_buf, 2, rx_buf);
	return rx_buf[1];
}

void sx1276_burstWrite(SX1276_t * chipAccess, uint8_t addr, uint8_t *buffer, uint8_t size)
{
	uint8_t tx_buf[size+1];
	uint8_t rx_buf[size+1];
	tx_buf[0] = AddWriteFlag(addr);
	memcpy(tx_buf+(1*sizeof(uint8_t)), buffer, size);
	spi_xfer_buf(chipAccess->spi, tx_buf, size+1, rx_buf);
}

void sx1276_burstRead(SX1276_t * chipAccess, uint8_t addr, uint8_t *buffer, uint8_t size)
{
	uint8_t tx_buf[size+1];
	uint8_t rx_buf[size+1];
	tx_buf[0] = AddReadFlag(addr);
	spi_xfer_buf(chipAccess->spi, tx_buf, size+1, rx_buf);
	memcpy(buffer, rx_buf+(1*sizeof(uint8_t)), size);
}

void sx1276_ios_init(SX1276_t * chipAccess)
{
	nrf_gpio_cfg_output(chipAccess->tcxo_pin);
	nrf_gpio_pin_set(chipAccess->tcxo_pin);
	nrf_gpio_cfg_input(chipAccess->dio_0_pin, NRF_GPIO_PIN_PULLDOWN);
	nrf_gpio_cfg_input(chipAccess->dio_1_pin, NRF_GPIO_PIN_PULLDOWN);
	nrf_gpio_cfg_input(chipAccess->dio_2_pin, NRF_GPIO_PIN_PULLDOWN);
	nrf_gpio_cfg_input(chipAccess->dio_3_pin, NRF_GPIO_PIN_PULLDOWN);
	
	nrf_gpio_cfg_output(chipAccess->ctx_pin);
	nrf_gpio_cfg_output(chipAccess->cps_pin);
	nrf_gpio_pin_set(chipAccess->ctx_pin);
	nrf_gpio_pin_clear(chipAccess->cps_pin);

	nrf_delay_ms(6);
}
void sx1276_reset(SX1276_t * chipAccess)
{
	nrf_gpio_cfg_output(chipAccess->reset_pin);
	nrf_gpio_pin_clear(chipAccess->reset_pin);
	nrf_delay_ms(1);
	nrf_gpio_cfg_input(chipAccess->reset_pin, NRF_GPIO_PIN_PULLUP);
	nrf_delay_ms(10);
}

void sx1276_rfSwitch(SX1276_t * chipAccess)
{
	switch(chipAccess->rf_setting.rf_switch_state)
	{
		case RF_TX_RUNNING:
		{
			nrf_gpio_pin_clear(chipAccess->ctx_pin);
			nrf_gpio_pin_set(chipAccess->cps_pin);
		} break;
		
		case RF_RX_RUNNING:
		{
			nrf_gpio_pin_set(chipAccess->ctx_pin);
			nrf_gpio_pin_clear(chipAccess->cps_pin);
		} break;
	}
}

uint32_t sx1276_setFrequency(SX1276_t * chipAccess, uint32_t freq)
{
	freq = (uint32_t)((double)freq / (double)FREQ_STEP);
	sx1276_write(chipAccess, REG_LR_FRFMSB, (uint8_t)((freq >> 16) & 0xFF));
	sx1276_write(chipAccess, REG_LR_FRFMID, (uint8_t)((freq >> 8) & 0xFF));
	sx1276_write(chipAccess, REG_LR_FRFLSB, (uint8_t)(freq & 0xFF));
	return freq;
}

uint32_t sx1276_getFrequency(SX1276_t * chipAccess)
{
	uint32_t freq = (sx1276_read(chipAccess, REG_LR_FRFMSB) << 16) | (sx1276_read(chipAccess, REG_LR_FRFMID) << 8) | sx1276_read(chipAccess, REG_LR_FRFLSB);
	return freq;
}
