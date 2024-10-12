
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void createTest (char* string, int argc, char *argv[]) {
    if (argc >= 2) {

        // Create uniq .txt
        sprintf(string, "./craeteNamessketo < names.%d_sorted.txt | sed '/q/d' | sort | uniq > __uniq__", atoi(argv[1]));
        system(string);
        system("echo q >> __uniq__");

        // Create test
        sprintf(string, "./newRngnames < __uniq__ > test%d", atoi(argv[1]));
        system(string);
        sprintf(string, "./create_fbn_txt < __uniq__ >> test%d", atoi(argv[1]));
        system(string);
        sprintf(string, "./project2 %d %d %d < test%d > out%d", atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[1]), atoi(argv[1]));
        system(string);
        sprintf(string, "rm test%d", atoi(argv[1]));
        system(string);
        system("rm __uniq__");
        system("rm create_fbn_txt");
        system("rm craeteNamessketo");
        system("rm newRngnames");
        system("rm makeExcelOut");
    }
}

// Any file DnE system's got your back

int main (int argc, char *argv[]) {
    char sysStr[200];
    FILE *file;

    if((file = fopen("project2.c","r"))!=NULL) {
        // file exists
        fclose(file);
    }
    else return(1);

    // Create executables
    system("gcc -Wall -g project2.c -o project2");
    system("gcc -Wall -g newRngnames.c -o newRngnames");
    system("gcc -Wall -g create_fbn_txt.c -o create_fbn_txt");
    system("gcc -Wall -g craeteNamessketo.c -o craeteNamessketo");

    // Arguments
    if (argc != 5) {
        printf("Missing arguments - Syntax: ./makeExcelOut >entries< >project2_argument1< >project2_argument2< >project2_argument3<");
        return(1);
    }

    // Create the test
    createTest(sysStr, argc, argv);

    return(0);
}