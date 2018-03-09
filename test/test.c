#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static void test(char* command, char* expected) {

    FILE *stream = popen(command, "r");
    char actual[2048] = "";
    // char *position = actual;
    char buf[256];
    while (fgets(buf, sizeof(buf), stream) != 0) {
        strcat(actual, buf);
    }

    pclose(stream);

    if(strcmp(actual, expected) != 0) {
        printf("Expected \"%s\" but received \"%s\"\n", expected, actual);
    }
}

int main() {
    // No file test
    test("../a.out", "No file provided!\n");

    // File doesn't exist
    test("../a.out aaslkdjfalkdjflkajsdf", "Invalid file!\n");

    // Test file without permissions
    system("touch no-access.csv");
    system("chmod 000 no-access.csv");
    test("../a.out no-access.csv", "Invalid file!\n");
    system("rm -f no-access.csv");

    // Test giving it a binary file
    test("../a.out ./a.out", "No name field in file!\n");

    // Successful run
    char *expected = "\"JetBlueNews\": 63\n"
                    "\"otisday\": 28\n"
                    "\"_mhertz\": 26\n"
                    "\"rossj987\": 23\n"
                    "\"scoobydoo9749\": 21\n"
                    "\"ThatJasonEaton\": 18\n"
                    "\"kbosspotter\": 17\n"
                    "\"SMHillman\": 17\n"
                    "\"MeeestarCoke\": 17\n"
                    "\"patrick_maness\": 16\n";
    test("../a.out cl-tweets-short.csv", expected);
}