#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"

int main(void) {
    // Enable clock for GPIO
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

    // Configure PC13 as push-pull output (e.g., LED)
    GPIO->P[2].MODEL &= ~(0xF << (4 * (13 % 8)));
    GPIO->P[2].MODEL |=  (0x4 << (4 * (13 % 8)));

    while (1) {
        GPIO->P[2].DOUTTGL = (1 << 13); // Toggle LED
        for (volatile int i = 0; i < 100000; i++); // Delay
    }
}
