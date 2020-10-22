#define _POSIX_C_SOURCE 200112L

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "base_64_encoder.h"
#include "base_64_decoder.h"


#define SUCCESS 0
#define ERROR 1


int main(int argc, char const *argv[]){ 
	if (argc > 6 || argc < 2){
		fprintf(stderr,"%s \n","Invalid command");
		return ERROR;
	}else{
		if (argc == 2){
			if (strcmp(argv[1],"-V") == 0){
				fprintf(stdout,"%s \n", "Version 1.2");
				return SUCCESS;
			}
			if (strcmp(argv[1],"-h") == 0){
				fprintf(stdout, "%s \n", "Usage:");
				fprintf(stdout, "%s \n", "tp0 -h");
				fprintf(stdout, "%s \n", "tp0 -V");
				fprintf(stdout, "%s \n", "tp0 [options]");
				fprintf(stdout, "%s \n", "-V, --version Print version and quit.");
				fprintf(stdout, "%s \n", "-h, --help Print this information.");
				fprintf(stdout, "%s \n", "-o, --output Path to output file.");
				fprintf(stdout, "%s \n", "-i, --input Path to input file.");
				fprintf(stdout, "%s \n", "-d, --decode Decode a base64-encoded file.");
				return SUCCESS;
			}
			fprintf(stderr, "%s \n", "Invalid command");
			return ERROR;
		}
		char* input_file;
		char* output_file;
		bool decode = false;
		bool error = false;
		int offset = 0;
		if (strcmp(argv[1],"-d") == 0){
			decode = true;
			offset = 1;
		}
		if (strcmp(argv[1 + offset],"-i") == 0){
			input_file = argv[2 + offset];
			output_file = argv[4 + offset];
		}
		else if (strcmp(argv[1 + offset],"-o") == 0){
			input_file = NULL;
			output_file = argv[2 + offset];
		}else{
			fprintf(stderr, "%s \n", "Invalid command");
			return ERROR;
		}
		if (decode){
			base_64_decoder_t decoder;
			if (base_64_decoder_init(&decoder, input_file, output_file)==ERROR){
				fprintf(stderr, "%s \n", "Error initiating decoder");
				return ERROR;
			}
			if (base_64_decoder_decode(&decoder) == ERROR)
				error = true;
			base_64_decoder_destroy(&decoder);
			if (error){
				fprintf(stderr, "%s \n", "Error decoding");
				return ERROR;
			}
			return SUCCESS;
		}  
		else{
			base_64_encoder_t encoder;
			if (base_64_encoder_init(&encoder, input_file, output_file) == ERROR){
				fprintf(stderr, "%s \n", "Error initiating encoder");
				return ERROR;
			}
			if (base_64_encoder_encode(&encoder) == ERROR)
				error = true;
			base_64_encoder_destroy(&encoder);
			if (error){
				fprintf(stderr, "%s \n", "Error decoding");
				return ERROR;
			}
			return SUCCESS;
		}
	}
}
