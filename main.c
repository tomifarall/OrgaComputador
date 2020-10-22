#define _POSIX_C_SOURCE 200112L

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include "base_64_encoder.h"
#include "base_64_decoder.h"


#define SUCCESS 0
#define ERROR -1

void display_help(){
	fprintf(stdout, "%s \n", "Usage:");
	fprintf(stdout, "%s \n", "tp0 -h");
	fprintf(stdout, "%s \n", "tp0 -V");
	fprintf(stdout, "%s \n", "tp0 [options]");
	fprintf(stdout, "%s \n", "-V, --version Print version and quit.");
	fprintf(stdout, "%s \n", "-h, --help Print this information.");
	fprintf(stdout, "%s \n", "-o, --output Path to output file.");
	fprintf(stdout, "%s \n", "-i, --input Path to input file.");
	fprintf(stdout, "%s \n", "-d, --decode Decode a base64-encoded file.");
}

int main(int argc, char **argv){ 
	if (argc > 6){
		fprintf(stderr,"%s \n","Invalid command");
		return ERROR;
	}
	static struct option long_options[] = {
        {"input", required_argument, 0, 'i' },
        {"output", required_argument, 0, 'o' },
        {"decode", no_argument, 0, 'd' },
        {"help", no_argument, 0,'h'},
        {"version", no_argument, 0, 'V'},
    };
	char* input_file = NULL;
	char* output_file = NULL;
	int option_index = 0;
	bool decode = false;
	int option;
	while ((option = getopt_long(argc, argv, "hVi:o:d", long_options, &option_index)) != -1) {
    	switch (option) {
    		case 'i':
    			input_file = optarg;
    			break;
    		case 'o':
    			output_file = optarg;
    			break;
    		case 'h':
    			display_help();
    			return SUCCESS;
    		case 'V':
    			fprintf(stdout,"%s \n", "Version 1.2");
    			return SUCCESS;
            case 'd':
                decode = true;
                break;
            default:
            	fprintf(stderr, "%s \n", "Invalid command");
				return ERROR;
        }
    }
    bool error = false;
	if (decode){
		base_64_decoder_t decoder;
		if (base_64_decoder_init(&decoder, input_file, output_file) == ERROR){
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
	}else{
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
