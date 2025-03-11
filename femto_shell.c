#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("My Awesome Shell > ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\nGood Bye :)\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            printf("Good Bye :)\n");
            break;
        }

        else if (strncmp(input, "echo", 4) == 0) {
            printf("%s\n", input + 5);
        }

        // Invalid command
        else {
            printf("Invalid command\n");
        }
    }

    return 0;
}
