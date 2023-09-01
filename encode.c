#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "encode.h"

//Функция конвертора из CP-1251 и ISO-8859-5 в UTF-8
int cp_iso(char *name_infile, char *name_outfile, char code) {
    FILE *infile = fopen(name_infile, "rb");
    if (!infile) {
        printf("Failed to open file\n");
	    return 1;
    }
    FILE* outfile = fopen(name_outfile, "w");
    if (outfile == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    int ch;
    unsigned char new_byte[2];
    while ((ch = fgetc(infile)) != EOF) {
        if(ch >= 0xB0) {
            if((code == 'c' || code == 'C') && (ch >= 0xC0) && (ch <= 0xEF)) {
                byte_change(new_byte, ch, DIF_CP_UTF1);
                fprintf(outfile, "%s", new_byte);
            }
            else if((code == 'c' || code == 'C') && (ch >= 0xF0)) {
                byte_change(new_byte, ch, DIF_CP_UTF2);
                fprintf(outfile, "%s", new_byte);
            }
            else if((code == 'i' || code == 'I') && (ch >= 0xB0) && (ch <= 0xDF)) {
                byte_change(new_byte, ch, DIF_ISO_UTF1);
                fprintf(outfile, "%s", new_byte);
            }
            else if((code == 'i' || code == 'I') && (ch >= 0xE0) && (ch <= 0xEF)) {
                byte_change(new_byte, ch, DIF_ISO_UTF2);
                fprintf(outfile, "%s", new_byte);
            }
            else {
                fputc(ch, outfile);
            }
        }
        else {
            fputc(ch, outfile);
        }
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}

//Замена нужных байтов из CP-1251, ISO-8859-5 и KOI8-R в UTF-8
unsigned char *byte_change(unsigned char *new_byte, uint16_t byte, unsigned int diff) {
    unsigned int sum = (unsigned int)(byte) + diff;
    new_byte[0] = (sum >> 8) & 0xFF;
    new_byte[1] = sum & 0xFF;
    return new_byte;
}

//Функция конвертора из KOI8-R в UTF-8
int koi(char *name_infile, char *name_outfile) {
    FILE *infile = fopen(name_infile, "rb");
    if (!infile) {
        printf("Failed to open file\n");
	    return 1;
    }
    FILE* outfile = fopen(name_outfile, "w");
    if (outfile == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    int ch;
    unsigned char new_byte[2];
    //В этом массиве в элементе с индексом символа (если начинать с ю) из таблицы KOI8-R 
    //содержится значение этого символа из таблицы UTF-8
    uint16_t match_koi_utf[128] = {0xD18E,0xD0B0,0xD0B1,0xD186,0xD0B4,
    0xD0B5,0xD184,0xD0B3,0xD185,0xD0B8,0xD0B9,0xD0BA,0xD0BB,0xD0BC,0xD0BD,0xD0BE,
    0xD0BF,0xD18F,0xD180,0xD181,0xD182,0xD183,0xD0B6,0xD0B2,0xD18C,0xD18B,0xD0B7,
    0xD188,0xD18D,0xD189,0xD187,0xD18A,0xD0AE,0xD090,0xD091,0xD0A6,0xD094,0xD095,
    0xD0A4,0xD093,0xD0A5,0xD098,0xD099,0xD09A,0xD09B,0xD09C,0xD09D,0xD09E,0xD09F,
    0xD0AF,0xD0A0,0xD0A1,0xD0A2,0xD0A3,0xD096,0xD092,0xD0AC,0xD0AB,0xD097,0xD0A8,
    0xD0AD,0xD0A9,0xD0A7,0xD0AA};
    while ((ch = fgetc(infile)) != EOF) {
        if(ch >= KOI_MIN) {
            unsigned int select = KOI_MIN;
            while (select <= KOI_MAX) {
                if ((ch) == (int)select) {
                    byte_change(new_byte, match_koi_utf[select - KOI_MIN], KOI_UTF);
                    fprintf(outfile, "%s", new_byte);
                }
                select+=0x01;
            }
        }
        else {
            fputc(ch, outfile);
        }
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}
