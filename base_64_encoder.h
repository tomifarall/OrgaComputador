#ifndef BASE_64_ENCODER_H
#define BASE_64_ENCODER_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "file_reader.h"

#define MASK_ENCODE 0x3F
#define LEN_SRC 3
#define LEN_DEST 4

typedef struct{
	file_reader_t file_reader;
	unsigned char src[LEN_SRC + 1];
	unsigned char dest[LEN_DEST + 1];
	FILE *out_file;
} base_64_encoder_t;

int base_64_encoder_init(base_64_encoder_t* self, char *file_to_encode, char *out_file);

int base_64_encoder_encode(base_64_encoder_t* self);

int base_64_encoder_destroy(base_64_encoder_t* self);

#endif