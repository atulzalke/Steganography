#define _GNU_SOURCE
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return ((width * height * 3));
}

/* Get File size
 * Input: File pointer
 * Output: text_size
 * Description
 */
uint get_file_size(FILE *fptr)
{
	int file_size;
	fseek(fptr, 0L, SEEK_END);
	file_size = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
	return file_size;
}


/* Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    //Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    //Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    //Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

uint get_bit(const char byte, int bit)
{
	return ((byte >> 8 - bit) & 1);
}

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	static int i;
	int len;
	char check[] = ".bmp";
	char check_txt[] = ".txt";
	char string[20], file_extn[10];
	char *f1, *f2, *f3;
	f1 = argv[2];
	f2 = argv[3];
	f3 = argv[4];

	// check 1st file
	len = strlen(f1);
	while(len > 0)
	{
		string[i] = f1[i];
		i++;
		len--;
	}
	//put manualy null to string
	string[i] = '\0';
	//revers the string file
	strrev(string);
	i=0;
	//copy extention of text file in file_ext
	while(string[i] != '.')
	{
		file_extn[i] = string[i];
		i++;
	}
	//at the end add '.'
	file_extn[i] = '.';
	i++;
	file_extn[i] = '\0';
	//get actual extension of file in file_ext
	strrev(file_extn);
	if (strcmp(file_extn, check) != 0)
		return e_failure;

	// check 2nd file
	i = 0;
	len = strlen(f2);
	while(len > 0)
	{
		string[i] = f2[i];
		i++;
		len--;
	}
	//put manualy null to string
	string[i] = '\0';
	//revers the string file
	strrev(string);
	i=0;
	//copy extention of text file in file_ext
	while(string[i] != '.')
	{
		file_extn[i] = string[i];
		i++;
	}
	//at the end add '.'
	file_extn[i] = '.';
	i++;
	file_extn[i] = '\0';
	//get actual extension of file in file_ext
	strrev(file_extn);
	if (strcmp(file_extn, check_txt) != 0)
		return e_failure;

	
	// check 3rd file
	i = 0;
	len = strlen(f3);
	while(len > 0)
	{
		string[i] = f3[i];
		i++;
		len--;
	}
	//put manualy null to string
	string[i] = '\0';
	//revers the string file
	strrev(string);
	i=0;
	//copy extention of text file in file_ext
	while(string[i] != '.')
	{
		file_extn[i] = string[i];
		i++;
	}
	//at the end add '.'
	file_extn[i] = '.';
	i++;
	file_extn[i] = '\0';
	//get actual extension of file in file_ext
	strrev(file_extn);
	if (strcmp(file_extn, check) != 0)
		return e_failure;

	return e_success;
}

/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
	int size_img = encInfo->bits_per_pixel;//get_image_size_for_bmp(encInfo->fptr_src_image);
	size_img /= 8;
	int size_txt = encInfo->size_secret_file;//get_file_size(encInfo->fptr_secret);

	if (size_img < size_txt)
	return e_failure;

	return e_success;
}

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	int count = 0;
	char buffer;
	rewind(fptr_src_image);
	while ( fread( &buffer, 1, 1, fptr_src_image) > 0)
	{
		count++;
		fwrite( &buffer, 1, 1, fptr_dest_image);
		if (count == 54)
			break;
	}
	if (count == 54)
		return e_success;
	else
		return e_failure;
}
///*  Encode Magic String size *///
Status encode_magic_string_size(int string_size, EncodeInfo *encInfo)
{
	
	char buffer = 0, msg_buff = 0;
	int i;
	int bit_msg;
	if ((ftell(encInfo->fptr_src_image)) != (ftell(encInfo->fptr_stego_image)))
	{
		printf("Not pointing at same location file_ptr and stego_ptr");
		return e_failure;
	}
//printf("magic_string file size: %d\n", string_size);
		for (i = 1; i <= 8; i++)
		{
			buffer = fgetc(encInfo->fptr_src_image);

			int byte_lsb = (buffer & 1);
			bit_msg = get_bit(string_size, i);
			if (byte_lsb == bit_msg)
			{
				fputc(buffer, encInfo->fptr_stego_image);
			}
			else
			{
				if (byte_lsb == 0)
					buffer = buffer | 1;
				else
					buffer = buffer ^ 1;

				fputc(buffer, encInfo->fptr_stego_image);
			}
		}

	if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
		return e_success;
	else
		return e_failure;
}


/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
	char buffer, msg_buff;
	int i, j=0;
	int bit_msg;

	while ((msg_buff = *(magic_string + j)) != '\0')
	{
		for (i = 1; i <=8; i++)
		{
			buffer = fgetc(encInfo->fptr_src_image);

			int byte_lsb = (buffer & 1);
			bit_msg = get_bit(msg_buff, i);
			if (byte_lsb == bit_msg)
			{
				fputc(buffer, encInfo->fptr_stego_image);
			}
			else
			{
				if (byte_lsb == 0)
					buffer = buffer | 1;
				else
					buffer = buffer ^ 1;

				fputc(buffer, encInfo->fptr_stego_image);
			}
		}
		j++;
	}
	if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
		return e_success;
	else
		return e_failure;
}

///* Encode secret file extenstion size*///
Status encode_secret_file_extn_size(int file_extn_size, EncodeInfo *encInfo)
{
	char buffer = 0, msg_buff = 0;
	int i;
	int bit_msg;
	if ((ftell(encInfo->fptr_src_image)) != (ftell(encInfo->fptr_stego_image)))
	{
		printf("Not pointing at same location file_ptr and stego_ptr");
		return e_failure;
	}
//printf("magic_string file size: %d\n", file_extn_size);
		for (i = 1; i <= 8; i++)
		{
			buffer = fgetc(encInfo->fptr_src_image);

			int byte_lsb = (buffer & 1);
			bit_msg = get_bit(file_extn_size, i);
			if (byte_lsb == bit_msg)
			{
				fputc(buffer, encInfo->fptr_stego_image);
			}
			else
			{
				if (byte_lsb == 0)
					buffer = buffer | 1;
				else
					buffer = buffer ^ 1;

				fputc(buffer, encInfo->fptr_stego_image);
			}
		}

	if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
		return e_success;
	else
		return e_failure;
}


/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	char buffer, msg_buff;
	int i, j=0;
	int bit_msg;
	if ((ftell(encInfo->fptr_src_image)) != (ftell(encInfo->fptr_stego_image)))
	{
		printf("Not pointing at same location file_ptr and stego_ptr");
		return e_failure;
	}
//printf("secret file extension: %s\n", file_extn);
	while ((msg_buff = file_extn[j]) != '\0')
	{
		for (i = 1; i <=8; i++)
		{
			buffer = fgetc(encInfo->fptr_src_image);

			int byte_lsb = (buffer & 1);
			bit_msg = get_bit(msg_buff, i);
			if (byte_lsb == bit_msg)
			{
				fputc(buffer, encInfo->fptr_stego_image);
			}
			else
			{
				if (byte_lsb == 0)
					buffer = buffer | 1;
				else
					buffer = buffer ^ 1;

				fputc(buffer, encInfo->fptr_stego_image);
			}
		}
		j++;
	}
	if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
		return e_success;
	else
		return e_failure;
	
}


/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{	
	char buffer = 0, msg_buff = 0;
	int i;
	int bit_msg;
	if ((ftell(encInfo->fptr_src_image)) != (ftell(encInfo->fptr_stego_image)))
	{
		printf("Not pointing at same location file_ptr and stego_ptr");
		return e_failure;
	}
//printf("Secret file size: %ld\n", file_size);
		for (i = 1; i <= 8; i++)
		{
			buffer = fgetc(encInfo->fptr_src_image);

			int byte_lsb = (buffer & 1);
			bit_msg = get_bit(file_size, i);
			if (byte_lsb == bit_msg)
			{
				fputc(buffer, encInfo->fptr_stego_image);
			}
			else
			{
				if (byte_lsb == 0)
					buffer = buffer | 1;
				else
					buffer = buffer ^ 1;

				fputc(buffer, encInfo->fptr_stego_image);
			}
		}

	if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
		return e_success;
	else
		return e_failure;
}

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	
	char buffer = 0, msg_buff = 0;
	int i;
	int bit_msg;

	if ((ftell(encInfo->fptr_src_image)) != (ftell(encInfo->fptr_stego_image)))
	{
		printf("Not pointing at same location file_ptr and stego_ptr");
		return e_failure;
	}

	while ((msg_buff = fgetc(encInfo->fptr_secret)) != EOF)
	{
		for (i = 1; i <= 8; i++)
		{
			buffer = fgetc(encInfo->fptr_src_image);

			int byte_lsb = (buffer & 1);
			bit_msg = get_bit(msg_buff, i);
			if (byte_lsb == bit_msg)
			{
				fputc(buffer, encInfo->fptr_stego_image);
			}
			else
			{
				if (byte_lsb == 0)
					buffer = buffer | 1;
				else
					buffer = buffer ^ 1;

				fputc(buffer, encInfo->fptr_stego_image);
			}
		}

	}
	if (ftell(encInfo->fptr_src_image) == ftell(encInfo->fptr_stego_image))
		return e_success;
	else
		return e_failure;
}

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	char buffer;
	if ((ftell(fptr_src)) != (ftell(fptr_dest)))
	{
		printf("Not pointing at same location file_ptr and stego_ptr");
		return e_failure;
	}
	while (fread(&buffer, 1, 1, fptr_src) > 0)
	{
		fwrite(&buffer, 1, 1, fptr_dest);
	}
	if ((ftell(fptr_src)) == (ftell(fptr_dest)))
		return e_success;
	else
		return e_failure;
}

/* Reverse the string */
void strrev(char *s)
{
	//take one char pointer in that put original pointer value 
	char *b = s;
	//till string present
	//loop will be continue
	while (*s)
	{
		//Increment the addres of strint by one
		s++;
	}
	//decrement last address
	//it contain null variable
	s--;
	//swap the string from an array
	while (b < s)
	{
		char temp = *b;
		*b = *s;
		*s = temp;
		b++;
		s--;
	}
}
