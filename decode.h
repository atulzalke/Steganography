#ifndef DECODE_H
#define DECODE_H

/* decode the size of string */
void size_decode(FILE *pf1, int *size_txt);
/* using size decode the string */
void string_decode(FILE *pf1, char *string, int size);
/* decode secret code */
void secret_code_decode(int size_txt, FILE *pf1, FILE *pf2);
/* Decode function */
int decode(char *argv_2, char *argv_3);

#endif
