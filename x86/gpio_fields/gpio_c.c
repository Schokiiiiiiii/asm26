#include <stdint.h>
#include <common.h>

#define GPIO_MODE_MASK   (0x3u << 0)    // bits 1:0
#define GPIO_SPEED_MASK  (0xFu << 2)    // bits 5:2
#define GPIO_PULLUP_MASK (0x1u << 6)    // bit 6
#define GPIO_ALT_MASK    (0x7u << 8)    // bits 10:8
#define GPIO_EN_MASK     (0x1u << 15)   // bit 15

uint32_t config_register(uint32_t reg)
{
    // clear fields
    reg &= ~GPIO_MODE_MASK;
    reg &= ~GPIO_SPEED_MASK;
    reg &= ~GPIO_PULLUP_MASK;
    reg &= ~GPIO_ALT_MASK;
    reg &= ~GPIO_EN_MASK;

    // set fields
    reg |= (2u << 0);   // MODE = 2
    reg |= (9u << 2);   // SPEED = 9
    reg |= (1u << 6);   // PULLUP = 1
    reg |= (5u << 8);   // ALT = 5
    reg |= (1u << 15);  // ENABLE = 1

    return reg;
}
