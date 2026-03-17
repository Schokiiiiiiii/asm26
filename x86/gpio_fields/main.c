#include <common.h>
#include <exports.h>
 
extern int config_register(volatile int gpio_reg);
extern int asm_config_register(volatile int gpio_reg);
 
/*
* 1. Les registres du GPIO sont à 0
* 2. Les registres du GPIO sont à 1
*/
void test_gpio_fields(void) {
	uint32_t gpio_c = 0;
	uint32_t gpio_s = 0;
 
	gpio_c = config_register(gpio_c);
	gpio_s = asm_config_register(gpio_s);
	printf("Test 1: %s\n", gpio_c == gpio_s ? "PASSED" : "FAILED");
	gpio_c = 0xFFFFFFFF;
	gpio_s = 0xFFFFFFFF;
	gpio_c = config_register(gpio_c);
	gpio_s = asm_config_register(gpio_s);
	printf("Test 2: %s\n", gpio_c == gpio_s ? "PASSED" : "FAILED");
}
 
int main(int argc, char *argv[]) {
 
	volatile int gpio_c = 0xc001;
	volatile int gpio_S = 0xc001;
 
	gpio_c = config_register(gpio_c);
	gpio_S = asm_config_register(gpio_S);
 
	if (gpio_c == gpio_S) {
		printf("Same result with assembler and C\n");
	}
	
	test_gpio_fields();
 
	printf("Hit any key to exit ... ");
	while (!tstc());
	
	/* consume input */
	(void) getc();
 
	printf("\n\n");
 
	return 0;
}