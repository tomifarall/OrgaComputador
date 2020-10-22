#include "base_64_decoder.h"

#define BYTES_CHUNK 4
#define OUTPUT_LEN 3

/* *****************************************************************
 *                   AUXILIARY FUNCTIONS                           *
 * *****************************************************************/ 

int is_valid_b64_char(char c) {
	if (c >= '0' && c <= '9')
		return 1;
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c >= 'a' && c <= 'z')
		return 1;
	if (c == '+' || c == '/' || c == '=')
		return 1;
	return 0;
}

int b64_decode(const unsigned char *src, unsigned char *out, size_t outlen) {
	if (!src || !out) return ERROR;
	char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char aux[4]; 

	for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 64; j++) {
			if (src[i] == base[j]) {
            	aux[i] = j;
            	break;
        	}
			if (src[i] == '=') {
				aux[i] = 0;
			}
        }
    }
	
	out[0] = (aux[0] << 2) | ((aux[1] & 0x30) >> 4);
    out[1] = ((aux[1] & 0xf) << 4) | ((aux[2] & 0x3c) >> 2);
    out[2] = ((aux[2] & 0x3) << 6) | aux[3];
	out[outlen] = '\0';

	return SUCCESS;
}

/* *****************************************************************
 *               BASE 64 DECODER FUNCTIONS                         *
 * *****************************************************************/

int base_64_decoder_init(base_64_decoder_t* self, char* file_to_decode, char* output_file) {
    if (!self) return ERROR;
    if (file_reader_init(&self->file_reader, file_to_decode) != SUCCESS) {
        return ERROR;
	}
	self->output_file = output_file ? fopen(output_file, "wt") : stdout;
	if (!self->output_file) { 
		file_reader_destroy(&(self->file_reader));
		return ERROR;
	}
    self->bytes_encoded = malloc(sizeof(unsigned char) * (BYTES_CHUNK + 1));
    if (!self->bytes_encoded) {
        file_reader_destroy(&self->file_reader);
		if (self->output_file != stdout) { 
			fclose(self->output_file);
		}
        return ERROR;
    }
    self->bytes_decoded = malloc(sizeof(unsigned char) * (OUTPUT_LEN + 1));
    if (!self->bytes_decoded) {
        free(self->bytes_encoded);
		if (self->output_file != stdout) {
			fclose(self->output_file);
		}
        file_reader_destroy(&self->file_reader);
        return ERROR;
    }
    memset(self->bytes_encoded, 0, BYTES_CHUNK +1);
    memset(self->bytes_decoded, 0, OUTPUT_LEN +1);
    return SUCCESS;
}


int base_64_decoder_decode(base_64_decoder_t* self) {
    if (!self) return ERROR;
	int total_bytes_read;
	int continue_encoding = 1;
    while (continue_encoding) {
        if (file_reader_get_bytes(&self->file_reader, self->bytes_encoded, BYTES_CHUNK, &total_bytes_read) == FILE_EOF) {
			continue_encoding = 0;
		}
		if (total_bytes_read != 0) {
			for (int i = 0; i < BYTES_CHUNK; i++) {
            	if (!is_valid_b64_char(self->bytes_encoded[i])) {
                	return ERROR;
        		}
			}
			memset(self->bytes_decoded, 0, OUTPUT_LEN +1);
        	b64_decode(self->bytes_encoded, self->bytes_decoded, OUTPUT_LEN);
			fputs((char *) self->bytes_decoded, self->output_file);
		}
    }
    fputs("\n", self->output_file);
    return SUCCESS;
}


int base_64_decoder_destroy(base_64_decoder_t* self) {
    if (!self) return ERROR;
    file_reader_destroy(&self->file_reader);
	if (self->output_file != stdout) {
		fclose(self->output_file);
	}
    free(self->bytes_encoded);
    free(self->bytes_decoded);
    return SUCCESS;
}



// int main(int argc, char const *argv[])
// {

//     base_64_decoder_t base_64_decoder;

//     base_64_decoder_init(&base_64_decoder, "test.txt", "output.txt");

//     base_64_decoder_decode(&base_64_decoder);

//     base_64_decoder_destroy(&base_64_decoder);

//     return 0;
// }