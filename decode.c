#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>

/* size decode*/
void size_decode(FILE *pf1, int *size_txt)
{
	int buffer = 0, i;
	int ch, bit_msg;
	for (i = 0; i < 8; i++)
	{
		ch = fgetc(pf1);
		bit_msg = (ch & 1);
		if (bit_msg)
		{
//			putchar('1');
			buffer = (buffer << 1) | 1;
		}
		else
		{
//			putchar('0');
			buffer = buffer << 1;
		}
	}
//	putchar('\n');
//	printf("buffer: %d\n", buffer);
//	printf("file pointe: %ld", ftell(pf1));
//	putchar('\n');
	*size_txt = buffer;
}

/* string decode */
void string_decode(FILE *pf1, char *string, int size)
{
	unsigned int buffer = 0, i, j =0, k = 0;
	int ch, bit_msg;
	for (i = 0; i < (size * 8); i++)
	{
		j++;
		ch = fgetc(pf1);
		bit_msg = (ch & 1);
		if (bit_msg)
			buffer = (buffer << 1) | 1;
		else
			buffer = buffer << 1;	

		if (j == 8)
		{
			string[k] = (char)buffer;
			j = 0;
			k++;
			buffer = 0;
		}
	}
	string[k] = '\0';
}

/* Secret massage decode */
void secret_code_decode(int size_txt, FILE *pf1, FILE *pf2)
{
	int buffer = 0, i, j =0, k = 0;
	int ch, bit_msg;
	char output[250] = {0};
	for (i = 0; i < (size_txt * 8); i++)
	{
		j++;
		ch = fgetc(pf1);
		bit_msg = (ch & 1);
		if (bit_msg)
		{
			buffer = (buffer << 1) | 1;
		}
		else
		{
			buffer = buffer << 1;	
		}

		if (j == 8)
		{
			putc(buffer, pf2);
			output[k++] = buffer;
			j = 0;
			buffer = 0;
		}
	}
	printf("\nSecret text is:\n%s\n\n", output);
}

int decode(char *argv_2, char *argv_3)
{
	FILE *pf1, *pf2;
	
	int size_magic_strn, size_file_extn, size_secret_file;

	//opening Image File
	if((pf1 = fopen(argv_2, "r")) == NULL) 
	{
		printf("Could not open file %s.\nAborting\n", argv_2);
		return 1;
	}

	fseek(pf1,54 ,SEEK_SET);
	
	if((pf2 = fopen(argv_3, "w+")) == NULL) 
	{
		printf("Could not open file %s.\nAborting\n", argv_3);
		return 1;
	}

	//decode magic string
	char magic_strn[20], magic_strn1[20], string[20];
	int j;
	size_decode(pf1, &size_magic_strn);
	string_decode(pf1, magic_strn, size_magic_strn);
//	string_decode(pf1, magic_strn, 2);
	printf("magic string: %s\n", magic_strn);
//	printf("magic_string size: %d\n", size_magic_strn);

//	printf("\nEnter the  magic string : ");
//	for(j = 0;(magic_strn1[j] = getchar()) != '\n'; j++);
//	magic_strn1[j]= '\0';
//printf("magic_string is: %s\n", magic_strn1);

	int temp;
//	temp = strcmp(magic_strn1, magic_strn);
/*
	if(temp)
	{
		printf("## Entered Wrong Magic String ## \n");
		return 0;
	}
	else
		printf("** Magic String Accepted ** \n");
*/	
	/* decode_secret_file_extn  */
	size_decode(pf1, &size_file_extn);
	printf("file extn size: %d\n", size_file_extn);
	string_decode(pf1, string, size_file_extn);
	printf("fiele extn: %s\n", string);

	/*Secret Text */
	size_decode(pf1, &size_secret_file);
	printf("size of secret file: %d\n", size_secret_file);
	secret_code_decode(size_secret_file, pf1, pf2);

	printf("Secret test is copy in  %s file\n", argv_3);

	/* closeing files */
	fclose(pf1);

}
