#include "base_64_encoder.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "file_reader.h"

#define SUCCESS 0
#define ERROR -1
#define FILE_EOF -2

/* *****************************************************************
 *                   AUXILIARY FUNCTIONS                           *
 * *****************************************************************/ 


int b64_encode(unsigned char *src, unsigned char *dest){
	if ((src == NULL) | (dest == NULL))
		return ERROR;
	char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

	for (size_t i = 0; i < LEN_DEST; i++){
		dest[LEN_DEST-(i+1)] = base[src[2] & MASK_ENCODE];
		for (size_t j = 0; j < LEN_SRC-1; j++){
			src[LEN_SRC-(j+1)] >>= 6;
			src[LEN_SRC-(j+1)] |= ((src[LEN_SRC-(j+2)] & MASK_ENCODE) << 2);
		}
		src[0] >>= 6;
	}
	dest[LEN_DEST] = '\0';
	return SUCCESS;
}

/* *****************************************************************
 *               BASE 64 DECODER FUNCTIONS                         *
 * *****************************************************************/


int base_64_encoder_init(base_64_encoder_t* self, char *file_to_encode, char *out_file){
	if (!self) return ERROR;
	if (file_reader_init(&(self->file_reader), file_to_encode) != SUCCESS) {
		return ERROR;
	}
	self->out_file = out_file != NULL ? fopen(out_file, "wt") : stdout;
	if (!self->out_file) {
		file_reader_destroy(&(self->file_reader));
		return ERROR;
	}
	return SUCCESS;
}

int base_64_encoder_encode(base_64_encoder_t* self){
	if (self == NULL)
		return ERROR;
	int total_bytes_read;
	int continue_encoding = 1;
	while (continue_encoding){
		if (file_reader_get_bytes(&(self->file_reader), self->src, LEN_SRC, &total_bytes_read) == FILE_EOF) {
			continue_encoding = 0;
		}
		if (total_bytes_read != 0) {
			b64_encode(self->src, self->dest);
			if (!continue_encoding){
				for (int i = LEN_DEST - 1; i > total_bytes_read; i--){
					self->dest[i] = '=';
				}
			}
			fputs((char *)self->dest, self->out_file);
		}
	}
	return SUCCESS;
}

int base_64_encoder_destroy(base_64_encoder_t* self){
	if (self == NULL)
		return ERROR;
	file_reader_destroy(&(self->file_reader));
	if (self->out_file != stdout){
		fclose(self->out_file);
	}
	return SUCCESS;
}
