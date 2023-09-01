#ifndef ENCODE_H
#define ENCODE_H

//В макросах хранится разница в коде для 1 и того же символа между кодировками
#define DIF_CP_UTF1 0xCFD0 //А-Я, а-п
#define DIF_CP_UTF2 0xD090 //р-я
#define DIF_ISO_UTF1 0xCFE0 //А-Я, а-п
#define DIF_ISO_UTF2 0xD0A0 //р-я
#define KOI_UTF 0 //Нужное значение сразу хранится в массиве
#define KOI_MIN 0xC0
#define KOI_MAX 0xFF

int cp_iso(char *name_infile, char *name_outfile, char code);
unsigned char *byte_change(unsigned char *new_byte, uint16_t byte, unsigned int diff);
int koi(char *name_infile, char *name_outfile);
#endif /* ENCODE_H */
