
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    char *pos_ptr = NULL;
    char *pos_ptr2 = NULL;
    char stringForCheck[20] = {"zeimai enas a kai gz"};
    char copytothis_str[6] = {0};
    if (argc != 3) {
        printf("Problem!\n");
        return 1;
    }
    pos_ptr = strpbrk(stringForCheck, argv[2]);
    if (pos_ptr == NULL) {
        printf("NULL");
    }
    else {
        pos_ptr2 = strpbrk(pos_ptr+1, argv[2]);
    }
    strncat(copytothis_str, pos_ptr+1, 5);
    for (pos_ptr++; pos_ptr < pos_ptr2; pos_ptr++) {
        *pos_ptr = toupper(*pos_ptr);
    }
    printf("%s\n", copytothis_str);
    printf("%s\n", stringForCheck);

    return(0);
}

