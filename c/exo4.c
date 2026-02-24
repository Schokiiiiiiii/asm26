/*
 * exo4.c - Debug an optimized code
 *
 * Author(s): Your Name(s)
 */

#include <stdio.h>

int main(int argc, char** argv){
	// create pointer
	int *p;

	// 0 -> 1 -> end
	for(int i = 0; i < 2; ++i){
		// if 0 -> create buffer and link it
		if (i == 0) {
			int buf[4] = {1,2,3,4};
			p = buf;
			break;
		}
	}

	// print all members inside pointer
	for (int i = 0; i < 4; ++i) {
		printf("p[%d] = %d\n", i, p[i]);
	}

	return 0;
}
