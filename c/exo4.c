/*
 * exo4.c - Debug an optimized code
 *
 * Author(s): Your Name(s)
 */

 #include <stdio.h>

 int main(int argc, char** argv){
    int *p;
    
    for(int i = 0; i < 2; ++i){
        if (i == 0) {
            int buf[4] = {1, 2, 3 ,4};
            p = buf;
            break;
        }
    }

    for (int i = 0; i < 4; ++i) {
        printf("p[%d] = %d\n", i, p[i]);
    }

    return 0;
 }
