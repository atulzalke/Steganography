#define _GNU_SOURCE
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include "decode.h"
#include "common.h"
int main(int argc, char *argv[])
{
	static int i=0;
	int len;
	char string[20], file_ext[10];
	char *f2;
	switch(argc)
	{
	case 5:
		//asign the second file name to f2 pointer
		f2 = argv[3];
		//calculate the lenght of text file
		len = strlen(f2);
		// copy element by element in string file
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
			file_ext[i] = string[i];
			i++;
		}
		//at the end add '.'
		file_ext[i] = '.';
		i++;
		file_ext[i] = '\0';
		//get actual extension of file in file_ext
		strrev(file_ext);

		//Start encoding
		if (!strcmp(argv[1],"-e"))
		{

		    EncodeInfo encInfo;
		    uint img_size, file_size;
//		    char magic_string[] = "#*";

		    // Fill with sample filenames
		    encInfo.src_image_fname = argv[2]; //"beautiful.bmp";
		    encInfo.secret_fname = argv[3]; //"secret.txt";
		    encInfo.stego_image_fname = argv[4]; //"stego_img.bmp";

			/*read and validate Encode args from argv */
		    if (read_and_validate_encode_args(argv, &encInfo) == e_failure)
			{
				printf("ERROR: %s function failed\n", "read_and_validate_encode_args" );
		    	return 1;
		    }
			else
		    {
		    	printf("SUCCESS: %s function completed\n", "read_and_validate_encode_args" );
			}
		
		    // Test open_files
		    if (open_files(&encInfo) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "open_files" );
		    	return 1;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "open_files" );
		    }

		    // Test get_image_size_for_bmp
		    img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
			encInfo.bits_per_pixel = img_size;
		    printf("INFO: Image size = %u\n", encInfo.bits_per_pixel);

		    // Test get_file_size
		    file_size = get_file_size(encInfo.fptr_secret);
		    encInfo.size_secret_file = file_size;
		    printf("INFO: Secret File size = %lu\n", encInfo.size_secret_file);
		    
		    // Test check_capacity 
		    if (check_capacity(&encInfo) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "check_capacity" );
		    	return 2;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "check_capacity" );
		    }

		    // Test copy_bmp_header
		    if (copy_bmp_header(encInfo.fptr_src_image, encInfo.fptr_stego_image) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "copy_bmp_header" );
		    	return 3;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "copy_bmp_header" );
		    }

		    //test encode_magic_string_size
//		    int string_size = strlen(magic_string);
		    int string_size = strlen(MAGIC_STRING);
		    if (encode_magic_string_size(string_size, &encInfo) == e_failure)
			{
		    	printf("ERROR: %s function failed\n", "encode_magic_string_size" );
		    	return 4;
			}
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "encode_magic_string_size" );
		    }

		    
		    // Test encode_magic_string
//		    if (encode_magic_string(magic_string, &encInfo) == e_failure)
		    if (encode_magic_string(MAGIC_STRING, &encInfo) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "encode_magic_string" );
		    	return 4;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "encode_magic_string" );
		    }
	    
		    // Test encode_secret_file_extn_size
		    int file_extn_size = strlen(file_ext);
		    if (encode_secret_file_extn_size(file_extn_size, &encInfo) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "encode_secret_file_extn_size" );
		    	return 5;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "encode_secret_file_extn_size" );
		    }
		    
		    // Test encode_secret_file_extn
		    if (encode_secret_file_extn(file_ext, &encInfo) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "encode_secret_file_extn" );
		    	return 5;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "encode_secret_file_extn" );
		    }
		    
		    // Test encode_secret_file_size
		    if (encode_secret_file_size(file_size, &encInfo) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "encode_secret_file_size" );
		    	return 5;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "encode_secret_file_size" );
		    }

		    // Test encode_secre_file_data
		    if (encode_secret_file_data(&encInfo) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "encode_secret_file_data" );
		    	return 6;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "encode_secret_file_data" );
		    }

		    
		    
		    
		    // Test copy_remaining_img_data
		    if (copy_remaining_img_data(encInfo.fptr_src_image, encInfo.fptr_stego_image) == e_failure)
		    {
		    	printf("ERROR: %s function failed\n", "copy_remaining_img_data");
		    	return 7;
		    }
		    else
		    {
		    	printf("SUCCESS: %s function completed\n", "copy_remaining_img_data");
		    }
		}
		else
			{
				printf("Please check delemiter\nIt must be '-e' for encoding and '-d' for decoding\n");
				exit(1);
			}
			fcloseall();
		break;
	case 4:
		//Start decoding
		if (!strcmp(argv[1],"-d"))
		{
			decode(argv[2], argv[3]);
		}
		break;

	default:
		printf("Enetr properly\n");
		exit(1);
	}

    return 0;
}
