#include <stdio.h>
#include <string.h>
void test(char* command, char* expected) {

    FILE *stream = popen(command, "r");
    char buf[256];
    while (fgets(buf, sizeof(buf), stream) != 0) {

    }
    pclose(stream);

    if(strcmp(buf, expected) != 0) {
        printf("Expected \"%s\" but received \"%s\"\n", expected, buf);
    }
}

void main() {
    // No file test
    test("./a.out", "No file provided!\n");
    // File doesn't exist
    test("./a.out aaslkdjfalkdjflkajsdf", "Invalid file!\n");
}