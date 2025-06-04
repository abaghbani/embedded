#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"

int main(void)
{
    // Enable GPIO clock
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Configure PE14 as push-pull output
    GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 1);

    // Configure PA15 as push-pull output
    GPIO_PinModeSet(gpioPortA, 15, gpioModePushPull, 1);

    // Configure PE15 as push-pull output
    GPIO_PinModeSet(gpioPortE, 15, gpioModePushPull, 1);

    // Example: toggle pins in a loop
    while (1) {
        GPIO_PinOutToggle(gpioPortA, 15);
        // GPIO_PinOutToggle(gpioPortA, 15);
        // GPIO_PinOutToggle(gpioPortE, 15);

        for (volatile int i = 0; i < 500000; ++i);  // crude delay
    }
    
}