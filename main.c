#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "encode.h"

int main(int argc, char *argv[]) {
    int size;
    unsigned char *text = NULL;
    unsigned char *utf_text = NULL;
    struct args *arg = arg_processing(argc, argv);
    if (arg == NULL) {
        return 1;
    }
    text = read_file(arg->infile, &size);
    if (text == NULL) {
        free_mem(arg, text, utf_text);
        return 1;
    }
    if (arg->code == 'k' || arg->code == 'K') {
        utf_text = koi(text, size);
    }
    else {
        utf_text = cp_iso(text, size, arg->code);
    }
    if (utf_text == NULL) {
        free_mem(arg, text, utf_text);
        return 1;
    }
    //printf("%s\n",utf_text);
    write_file(utf_text, arg->outfile);
    free_mem(arg, text, utf_text);
    return 0;
}
