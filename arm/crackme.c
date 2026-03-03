#include <common.h>
#include <exports.h>

#define MY_HASH 0xDEADBEEF
#define MAX_SIZE 255

/* Compare the hash of the input with the hash of the secret password */
unsigned int check_password(char *pw) {
        unsigned int hash = 5381;

        /* Compute the hash of the input */
        while (*pw)
                hash = 33 * hash ^ (unsigned char) *pw++;

        return (hash == MY_HASH);
}

int main(int argc, char *argv[]) {
        unsigned int i = 0;
        char password[MAX_SIZE + 1];

        printf("Enter password : \n");

        /* consume input */
        while (i < MAX_SIZE) {
                while (!tstc());
                password[i] = getc();
                if ((password[i] == '\n') || password[i] == '\r')
                        break;
                printf("%c", password[i]);
                i++;
        }
        printf("\n");
        /* Add the null terminator */
        password[i] = 0;

        printf("The password is beeing checked... ");

        if (check_password(password)) {
        		printf("Correct password ! \n");
                printf("Good job ! Enjoy !\n");
        } else {
                printf("Incorrect password !\n");
                return -1;
        }

        return 0;
}