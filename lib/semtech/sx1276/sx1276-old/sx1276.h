#ifndef __SX1276_H__
#define __SX1276_H__

#include "sx1276Regs-Fsk.h"
#include "sx1276Regs-LoRa.h"
#include "spi_driver.h"

#define SPI_WRITE_FLAG			0x80
#define SPI_READ_FLAG			0x7F
#define AddWriteFlag(addr)		addr | SPI_WRITE_FLAG
#define AddReadFlag(addr)		addr & SPI_READ_FLAG

#define XTAL_FREQ				32000000.0
#define FREQ_STEP				(XTAL_FREQ/524288.0)

typedef enum
{
	RF_RX_RUNNING = 0,
	RF_TX_RUNNING,
} Rf_Switch_t;

typedef struct
{
	Rf_Switch_t			rf_switch_state;
	uint32_t			frequency;
	uint8_t				lna_gain;
	
} Rf_Setting_t;

typedef struct
{
	uint8_t				band_width;
	uint8_t				coding_rate;
	uint8_t				implicit_enable;
	uint8_t				spread_factor;
	uint8_t				tx_continous_enable;
	uint16_t			rx_symb_timeout;
	uint16_t			preamble_number;
} MODEM_Setting_t;

typedef struct
{
	Spi_t				*spi;
	uint8_t				dio_0_pin;
	uint8_t				dio_1_pin;
	uint8_t				dio_2_pin;
	uint8_t				dio_3_pin;
	uint8_t				reset_pin;
	uint8_t				tcxo_pin;
	uint8_t				ctx_pin;
	uint8_t				cps_pin;
	Rf_Setting_t		rf_setting;
	MODEM_Setting_t		modem;
} SX1276_t;

uint32_t sx1276_init(SX1276_t *chipAccess);
void sx1276_modem_setting(SX1276_t *chipAccess);
void sx1276_write(SX1276_t *chipAccess, uint8_t addr, uint8_t data);
uint8_t sx1276_read(SX1276_t *chipAccess, uint8_t addr);
void sx1276_burstWrite(SX1276_t *chipAccess, uint8_t addr, uint8_t *buffer, uint8_t size);
void sx1276_burstRead(SX1276_t *chipAccess, uint8_t addr, uint8_t *buffer, uint8_t size);
void sx1276_ios_init(SX1276_t *chipAccess);
void sx1276_reset(SX1276_t *chipAccess);
void sx1276_rfSwitch(SX1276_t *chipAccess);
void sx1276_transmit(SX1276_t *chipAccess, uint8_t *txData);
uint32_t sx1276_setFrequency(SX1276_t *chipAccess, uint32_t freq);
uint32_t sx1276_getFrequency(SX1276_t *chipAccess);
#endif