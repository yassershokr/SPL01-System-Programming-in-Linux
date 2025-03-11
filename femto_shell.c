#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("My Awesome Shell > ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            // End of file, possibly Ctrl+D
            printf("\nGood Bye :)\n");
            break;
        }

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = 0;

        // Check for 'exit' command
        if (strcmp(input, "exit") == 0) {
            printf("Good Bye :)\n");
            break;
        }

        // Check if the command starts with 'echo'
        else if (strncmp(input, "echo", 4) == 0) {
            // Print the rest of the input after 'echo '
            printf("%s\n", input + 5);
        }

        // Invalid command
        else {
            printf("Invalid command\n");
        }
    }

    return 0;
}