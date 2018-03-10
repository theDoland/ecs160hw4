#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 10000


static char* gen()
{
    char *dst;
    int i, n;  
    srand(time(NULL));               /* init seed */
    if ((dst = malloc(N * sizeof(char))) == NULL)   /* caller will need to free this */
    {
        printf("Failed to generate");
        exit(1);
    }
    for (i = 0; i < N; )
    {
        if ((n = rand() % 127) > 'A' && n < 'Z') {
            dst[i++] = n;
        }
    }       
 
    dst[N - 1] = '\0';                   /* null terminate the string */
    return dst;
}

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

    // Empty file
    system("touch empty.csv");
    test("../a.out empty.csv", "No name field in file!\n");
    system("rm -f empty.csv");

    // Test file without permissions
    system("touch no-access.csv");
    system("chmod 000 no-access.csv");
    test("../a.out no-access.csv", "Invalid file!\n");
    system("rm -f no-access.csv");

    // Test giving it a binary file
    test("../a.out ./a.out", "No name field in file!\n");

    // Long first line
    system("touch long.csv");
    char *longline = gen();
    FILE* fstream = fopen("long.csv", "w");
    fputs( longline, fstream );
    test("../a.out long.csv", "Line length exceeds limit!\n");
    system("rm -f long.csv");
    free(longline);

    test("../a.out tweets-bad-line.csv", "Invalid line!\n");

    test("../a.out single-column.csv", "\"JetBlueNews\": 2\n\"mrshossruns\": 1\n");

    // @Todo Check with windows line endings!
    // CSV with null characters?
    // blank lines in csv
    // test max length csv

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