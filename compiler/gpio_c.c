/* Ne pas modifier */

int config_register(volatile int gpio_reg) {
        unsigned int mask = 0xFF00A5A5;
        unsigned int upper_byte_mask = 0xFF000000;

        // Set bit 0 to 0
        gpio_reg &= ~(unsigned int)0x1;

        // Set bits 5,6,7, and 8 to 1
        gpio_reg |= 0xF << 4;

        // Apply mask
        gpio_reg = gpio_reg & mask;

        // Clear upper byte
        gpio_reg &= ~upper_byte_mask;

        // Set upper byte
        gpio_reg |= (0x66 << 3*8);

        // Divide value by 8
        gpio_reg /= 8;

        // Composite operation
        gpio_reg = (gpio_reg | 0xA5) & (0xFFEEDD << 3);

        return gpio_reg;
}