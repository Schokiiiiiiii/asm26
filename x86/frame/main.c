#include <common.h>
#include <exports.h>
 
extern uint32_t frame_build(uint32_t value);
extern uint32_t frame_check(uint32_t value);
extern uint32_t checksum_to_hex(uint32_t value);
 
/*
* 1. Test: frame_build: checksum est correct
* 2. Test: frame_build: format de retour est correct
* 3. Test: frame_check avec un checksum correct
* 4. Test: frame_check avec un checksum incorrect
* 5. Test: checksum_to_hex: pour un hex qui contient que des chiffres
* 6. Test: checksum_to_hex: pour un hex qui contient des lettres
*/
void tests_frame(void) {
	uint8_t byte2 = 0x56;
	uint8_t byte1 = 0x1A;
	uint8_t byte0 = 0x3E;
	uint32_t value = ((uint32_t)byte2 << 16) | ((uint32_t)byte1 << 8) | (uint32_t)byte0;
	uint8_t checksum = ~((byte2 + byte1 + byte0) % 256);
	uint32_t format = (value << 8) | checksum;
	uint32_t frame = frame_build(value);
	printf("Test 1: %s\n", (frame & 0xff) == checksum ? "PASSED" : "FAILED");
	printf("Test 2: %s\n", frame == format ? "PASSED" : "FAILED");
	uint32_t result = frame_check(frame);
	printf("Test 3: %s\n", (result == 0) ? "PASSED" : "FAILED");
	result = frame_check(frame | (0xEE << 8));
	printf("Test 4: %s\n", (result != 0) ? "PASSED" : "FAILED");
	uint32_t hex = (value << 8) | 0x39;
	result = checksum_to_hex(hex);
	printf("Test 5: %s\n", (result == 0x30783339) ? "PASSED" : "FAILED");
	hex = (value << 8) | 0xAF;
	printf("print %x\n", result);
	printf("Test 6: %s\n", (result == 0x30784146) ? "PASSED" : "FAILED");
}
 
int main(int argc, char *argv[]) {
	uint8_t byte2 = 0x13;
	uint8_t byte1 = 0x0A;
	uint8_t byte0 = 0xB2;
	uint32_t result;
	
	/* Create a word with the 3 bytes */
	uint32_t value = ((uint32_t)byte2 << 16) | ((uint32_t)byte1 << 8) | (uint32_t)byte0;
 
	uint32_t checksum = frame_build(value);
	result = frame_check(checksum);
	printf("result : %d\n", result);
	uint32_t hex = checksum_to_hex(checksum);
	printf("checksum to hex: %c%c%c%c\n", (hex >> 24 & 0xFF), (hex >> 16) & 0xff,  (hex >> 8) & 0xff, hex & 0xff);
	
	tests_frame();
	printf("Hit any key to exit ... ");
	while (!tstc());
	
	/* consume input */
	(void) getc();
 
	printf("\n\n");
 
	return 0;
}