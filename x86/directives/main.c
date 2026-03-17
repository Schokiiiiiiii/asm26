#include <common.h>
#include <exports.h>


extern int to_upper_case(char *string);

int main(int argc, char *argv[]) {
	int size = 0;

	char string[] = "Hello ASM !";

	size = to_upper_case(string);
        
	printf("Upper case: %s\n", string);
	printf("New size of the string: %d\n", size);

	printf("Hit any key to exit ... ");
	while (!tstc());
	
	/* consume input */
	(void) getc();

	printf("\n\n");

	return 0;
}
