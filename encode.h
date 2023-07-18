#ifndef ENCODE_H
#define ENCODE_H

struct args {
    char *infile;
    char code;
    char *outfile;
};

struct args *arg_processing(int argc, char *argv[]);
unsigned char *read_file(char *name_file, int *size);
int file_size(FILE *file);
unsigned char *cp_iso(unsigned char *text, int size, char code);
unsigned char *byte_change(unsigned char *utf_text, uint16_t byte, int *size, int *i, unsigned int diff);
unsigned char *koi(unsigned char *text, int size);
void write_file(unsigned char *utf_text, char *file_name);
void free_mem(struct args *arg, unsigned char *text, unsigned char *utf_text);
#endif /* ENCODE_H */
