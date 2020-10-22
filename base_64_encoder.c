#ifndef BASE_64_encoder_H
#define BASE_64_encoder_H

#include "base_64_encoder.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "file_reader.h"


int b64_encode(unsigned char *src, unsigned char *dest){
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
}


int base_64_encoder_init(base_64_encoder_t* self, char *file_to_encode, char *out_file){
	file_reader_init(&(self->file_reader), file_to_encode);
	self->out_file = out_file ? fopen(out_file, "wt") : stdout;
}

int base_64_encoder_encode(base_64_encoder_t* self){
	while (!(file_reader_end_of_file(&self->file_reader))){
		file_reader_get_bytes(&(self->file_reader), self->src, LEN_SRC);
		b64_encode(self->src, self->dest);
		fputs(self->dest, self->out_file);
	}
}

int base_64_encoder_destroy(base_64_encoder_t* self){
	file_reader_destroy(&(self->file_reader));
	if (self->out_file != stdout){
		fclose(self->out_file);
	}
}

int main(int argc, char const *argv[])
{
    base_64_encoder_t base_64_encoder;

    base_64_encoder_init(&base_64_encoder, NULL, "salida.txt");

    base_64_encoder_encode(&base_64_encoder);

    base_64_encoder_destroy(&base_64_encoder);

    return 0;
}

#endif