//В макросах хранится разница в коде для 1 и того же символа между кодировками
#define DIF_CP_UTF1 0xCFD0 //А-Я, а-п
#define DIF_CP_UTF2 0xD090 //р-я
#define DIF_ISO_UTF1 0xCFE0 //А-Я, а-п
#define DIF_ISO_UTF2 0xD0A0 //р-я

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "encode.h"

//Проверка входных аргументов и создание экземпляра структуры для их хранения
struct args *arg_processing(int argc, char *argv[]) {
    struct args *arg;
    if (argc != 4) {
        printf("Invalid number of arguments\n");
        return NULL;
    }
    if (strcmp(argv[2], "cp")!=0 && strcmp(argv[2], "cp-1251")!=0 && strcmp(argv[2], "CP-1251")!=0 &&
    strcmp(argv[2], "koi")!=0 && strcmp(argv[2], "koi8-r")!=0 && strcmp(argv[2], "KOI8-R")!=0 &&
    strcmp(argv[2], "iso")!=0 && strcmp(argv[2], "iso-8859-5")!=0 && strcmp(argv[2], "ISO-8859-5")!=0) {
        printf("Wrong encoding entered\n");
        return NULL;
    }
    else {
        arg = (struct args*)malloc(sizeof(struct args));
        if (arg == NULL) {
            printf("Memory allocation error\n");
            return NULL;
        }
        arg->infile = malloc(sizeof(char)*strlen(argv[1]));
        if (arg->infile == NULL) {
            printf("Memory allocation error\n");
            free(arg);
            return NULL;
        }
        arg->outfile = malloc(sizeof(char)*strlen(argv[3]));
        if (arg->outfile == NULL) {
            printf("Memory allocation error\n");
            free(arg->infile);
            free(arg);
            return NULL;
        }
        strcpy(arg->infile,argv[1]);
        strncpy(&arg->code,argv[2],1);
        strcpy(arg->outfile,argv[3]);
    }
    return arg;
}

//Считываем текст из документа в строку
unsigned char *read_file(char *name_file, int *size) {
    unsigned char *text;
    FILE *file = fopen(name_file, "rb");
    if (!file) {
        printf("Failed to open file\n");
	    return NULL;
    }
    *size = file_size(file);
    text = (unsigned char *)malloc(sizeof(unsigned char) * (*size + 1));
    if (text == NULL) {
        printf("Memory allocation error\n");
        fclose(file);
        return NULL;
    }
    int nbytes = fread(text, 1, *size, file);
    if (nbytes != *size) {
        printf("Failed to read data from file\n");
        fclose(file);
	    return NULL;
    }
    (*size)++;
    fclose(file);
    return text;
}

int file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);
    return size;
}

//Функция конвертора из CP-1251 и ISO-8859-5 в UTF-8
unsigned char *cp_iso(unsigned char *text, int size, char code) {
    unsigned char *utf_text = (unsigned char *)malloc(sizeof(unsigned char *) * size);
    if (utf_text == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }
    unsigned char *temp;
    int i = 0;
    while (*text != '\0') {
        if(*text >= 0xB0) {
            if((code == 'c' || code == 'C') && (*text >= 0xC0) && (*text <= 0xEF)) {
                temp = byte_change(utf_text, *text, &size, &i, DIF_CP_UTF1);
                if (temp == NULL) {
                    return NULL;
                } 
                utf_text = temp;
            }
            else if((code == 'c' || code == 'C') && (*text >= 0xF0)) {
                temp = byte_change(utf_text, *text, &size, &i, DIF_CP_UTF2);
                if (temp == NULL) {
                    return NULL;
                } 
                utf_text = temp;
            }
            else if((code == 'i' || code == 'I') && (*text >= 0xB0) && (*text <= 0xDF)) {
                temp = byte_change(utf_text, *text, &size, &i, DIF_ISO_UTF1);
                if (temp == NULL) {
                    return NULL;
                } 
                utf_text = temp;
            }
            else if((code == 'i' || code == 'I') && (*text >= 0xE0) && (*text <= 0xEF)) {
                temp = byte_change(utf_text, *text, &size, &i, DIF_ISO_UTF2);
                if (temp == NULL) {
                    return NULL;
                } 
                utf_text = temp;
            }
            else {
                utf_text[i] = *text;
                i++;
            }
        }
        else {
            utf_text[i] = *text;
            i++;
        }
        text++;
    }
    return utf_text;
}

//Замена нужных байтов из CP-1251 и ISO-8859-5 в UTF-8
unsigned char *byte_change(unsigned char *utf_text, unsigned char byte, int *size, int *i, unsigned int diff) {
    (*size)++;
    unsigned char *temp = (unsigned char *)realloc(utf_text, sizeof(unsigned char *) * (*size));
    if (temp == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    } 
    utf_text = temp;
    unsigned int sum = (unsigned int)(byte) + diff;
    utf_text[(*i)] = (sum >> 8) & 0xFF;
    utf_text[(*i) + 1] = sum & 0xFF;
    *i += 2;
    return utf_text;
}

//Функция конвертора из KOI8-R в UTF-8
unsigned char *koi(unsigned char *text, int size) {
    unsigned char *utf_text = (unsigned char *)malloc(sizeof(unsigned char *) * size);
    if (utf_text == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }
    int i = 0;
    //В этом массиве в элементе с индексом символа (если начинать с ю) из таблицы KOI8-R 
    //содержится значение этого символа из таблицы UTF-8
    uint16_t match_koi_utf[128] = {0xD18E,0xD0B0,0xD0B1,0xD186,0xD0B4,
    0xD0B5,0xD184,0xD0B3,0xD185,0xD0B8,0xD0B9,0xD0BA,0xD0BB,0xD0BC,0xD0BD,0xD0BE,
    0xD0BF,0xD18F,0xD180,0xD181,0xD182,0xD183,0xD0B6,0xD0B2,0xD18C,0xD18B,0xD0B7,
    0xD188,0xD18D,0xD189,0xD187,0xD18A,0xD0AE,0xD090,0xD091,0xD0A6,0xD094,0xD095,
    0xD0A4,0xD093,0xD0A5,0xD098,0xD099,0xD09A,0xD09B,0xD09C,0xD09D,0xD09E,0xD09F,
    0xD0AF,0xD0A0,0xD0A1,0xD0A2,0xD0A3,0xD096,0xD092,0xD0AC,0xD0AB,0xD097,0xD0A8,
    0xD0AD,0xD0A9,0xD0A7,0xD0AA};
    while (*text != '\0') {
        if(*text >= 0xC0) {
            unsigned int j = 0xC0;
            int k = 0;
            while (k < 64) {
                if ((*text) == j) {
                    size++;
                    unsigned char *temp = (unsigned char *)realloc(utf_text, sizeof(unsigned char *) * size);
                    if (temp == NULL) {
                        printf("Memory allocation error\n");
                        return NULL;
                    } 
                    utf_text = temp;
                    utf_text[i] = (match_koi_utf[k] >> 8) & 0xFF;
                    utf_text[i + 1] = match_koi_utf[k] & 0xFF;
                    i += 2;
                }
                j+=0x01;
                k++;
            }
        }
        else {
            utf_text[i] = *text;
            i++;
        }
        text++;
    }
    return utf_text;
}

void write_file(unsigned char *utf_text, char *file_name) {
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }
    fprintf(file, "%s", utf_text);
    fclose(file);
    printf("The text was successfully written to the file\n");
}

void free_mem(struct args *arg, unsigned char *text, unsigned char *utf_text) {
    free(arg->infile);
    free(arg->outfile);
    free(arg);
    if (text != NULL) {
        free(text);
    }
    if (utf_text != NULL) {
        free(utf_text);
    }
}
