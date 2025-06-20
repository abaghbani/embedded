// gpio definition on itap probe

#ifndef ITAP_H
#define ITAP_H

// --- BOARD SPECIFIC ---
#define ITAP_PROBE_LED_R 0
#define ITAP_PROBE_LED_G 1
#define ITAP_PROBE_LED_B 2

#define ITAP_PROBE_VIO_ADJ_PWM 3
#define ITAP_PROBE_TDO_O 4
#define ITAP_PROBE_TDI_O 5
#define ITAP_PROBE_TCK_O 6
#define ITAP_PROBE_TMS_O 7
#define ITAP_PROBE_TRN_OE_n 8
#define ITAP_PROBE_VTGT_EN 9
#define ITAP_PROBE_TMS_PU_EN 14
#define ITAP_PROBE_TCK_PU_EN 15

/* Internal I2C bus, HW I2C1 */
#define ITAP_PROBE_SDA 10
#define ITAP_PROBE_SCL 11
#define ITAP_PROBE_I2C_INST 1
#define ITAP_PROBE_I2C_EEPROM_ADDR 0x50
#define EEPROM_24LC08 1
#define ITAP_PROBE_I2C_EEPROM_TYPE EEPROM_24LC08

/* Internal Debug console, UART0 */
#define ITAP_PROBE_DBG_TX 12
#define ITAP_PROBE_DBG_RX 13
#define ITAP_PROBE_DBG_UART_INST 0
#define ITAP_PROBE_DUT_UART0_INST 0

// UART0 pins
#define ITAP_PROBE_UART0_TX 16
#define ITAP_PROBE_UART0_RX 17
#define ITAP_PROBE_DET_DRAIN 18
#define ITAP_PROBE_RST_DRAIN 19
// JTAG pins
#define ITAP_PROBE_TDO 20
#define ITAP_PROBE_TMS 21
#define ITAP_PROBE_TCK 22
#define ITAP_PROBE_TDI 23
// SPI0 pins
#define ITAP_PROBE_SPI0_RX 20
#define ITAP_PROBE_SPI0_CSN 21
#define ITAP_PROBE_SPI0_SCK 22
#define ITAP_PROBE_SPI0_TX 23

#define ITAP_PROBE_TCK_TX 24
#define ITAP_PROBE_DUT_UART_INST 0

#define ITAP_PROBE_ADC_TMS 26
#define ITAP_PROBE_ADC_VIO 27
#define ITAP_PROBE_ADC_VTGT 28
#define ITAP_PROBE_ADC_TDO 29

#define ITAP_PROBE_ADC_TMS_CH 0
#define ITAP_PROBE_ADC_VIO_CH 1
#define ITAP_PROBE_ADC_VTGT_CH 2
#define ITAP_PROBE_ADC_TDO_CH 3

void itap_led_set(int r, int g, int b);
void itap_led_init(void);
void itap_vref_init(void);
void itap_sensing_init(void);
int itap_sensing_mv_get(int channel);

#endif // ITAP_H
