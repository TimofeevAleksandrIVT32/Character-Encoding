#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "encode.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Invalid number of arguments\n");
        return 1;
    }
    if (strcmp(argv[2], "cp")!=0 && strcmp(argv[2], "cp-1251")!=0 && strcmp(argv[2], "CP-1251")!=0 &&
    strcmp(argv[2], "koi")!=0 && strcmp(argv[2], "koi8-r")!=0 && strcmp(argv[2], "KOI8-R")!=0 &&
    strcmp(argv[2], "iso")!=0 && strcmp(argv[2], "iso-8859-5")!=0 && strcmp(argv[2], "ISO-8859-5")!=0) {
        printf("Wrong encoding entered\n");
        return 1;
    }
    if (argv[2][0] == 'k' || argv[2][0] == 'K') {
        if (koi(argv[1], argv[3]) == 1){
            return 1;
        }
    }
    else {
        if (cp_iso(argv[1], argv[3], argv[2][0]) == 1){
            return 1;
        }
    }
    return 0;
}
