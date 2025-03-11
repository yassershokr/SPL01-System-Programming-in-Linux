#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
        return 1;
    }

    FILE *source = fopen(argv[1], "rb");
    if (source == NULL) {
        perror("Error opening source file");
        return 1;
    }

    FILE *destination = fopen(argv[2], "wb");
    if (destination == NULL) {
        perror("Error opening destination file");
        fclose(source);
        return 1;
    }

    char buffer[BUFSIZ];
    size_t bytes;
    while ((bytes = fread(buffer, 1, BUFSIZ, source)) > 0) {
        fwrite(buffer, 1, bytes, destination);
    }

    fclose(source);
    fclose(destination);
    return 0;
}