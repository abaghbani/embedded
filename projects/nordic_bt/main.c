#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"
#include "ble_dtm.h"
#include "boards.h"
#include "app_uart.h"
#include "nrf_delay.h"

int main(void)
{
	uint8_t			channel;
	//uint32_t		currentTime;
	
	nrf_gpio_range_cfg_output(8, 31);
    bsp_board_init(BSP_INIT_LEDS);
    
    if (dtm_init() != DTM_SUCCESS)
    {
        // If DTM cannot be correctly initialized, then we just return.
        return -1;
    }

	dtm_set_txpower(0);													// output level, +4..-40, in steps of 4.
	dtm_cmd(LE_TEST_SETUP, LE_TEST_SETUP_RESET, 0, 0);
	dtm_cmd(LE_TEST_SETUP, LE_TEST_SETUP_SET_UPPER, 0, 0);				// 0..3
	dtm_cmd(LE_TEST_SETUP, LE_TEST_SETUP_SET_PHY, LE_PHY_LE_CODED_S8, 0);		// LE_PHY_1M / LE_PHY_2M / LE_PHY_LE_CODED_S8 / LE_PHY_LE_CODED_S2
	
	bsp_board_led_on(BSP_BOARD_LED_0);
	nrf_delay_us(500000);
	bsp_board_led_off(BSP_BOARD_LED_0);
	nrf_delay_us(500000);
	bsp_board_led_on(BSP_BOARD_LED_0);
	
	channel = 0;
	while(1)
	{
		dtm_cmd(LE_TRANSMITTER_TEST, channel, 25, DTM_PKT_CUSTOMISED);			// ch:0..39 , length:0..37
		
		nrf_gpio_pin_write(ARDUINO_0_PIN, 1);
		nrf_gpio_pin_write(ARDUINO_1_PIN, 1);
		nrf_delay_us(5);
		nrf_gpio_pin_write(ARDUINO_0_PIN, 0);
		nrf_gpio_pin_write(ARDUINO_1_PIN, 0);
		
		dtm_wait();
		if(channel >= 40)
			channel = 0;
		//while(dtm_wait() == currentTime);
		
		// for(int i=0;i<150;i++)
		// {
		// nrf_gpio_pin_write(ARDUINO_0_PIN, 1);
		// nrf_gpio_pin_write(ARDUINO_1_PIN, 1);
		// nrf_delay_us(50);
		// nrf_gpio_pin_write(ARDUINO_0_PIN, 0);
		// nrf_gpio_pin_write(ARDUINO_1_PIN, 0);
		// }
	}
	
	// bsp_board_led_on(BSP_BOARD_LED_0);
	// for(int i=0;i<1000000;i++);
	// bsp_board_led_off(BSP_BOARD_LED_0);
	// for(int i=0;i<1000000;i++);
	// bsp_board_led_on(BSP_BOARD_LED_0);
	
	
}
