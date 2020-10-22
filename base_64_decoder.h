#ifndef BASE_64_DECODER_H
#define BASE_64_DECODER_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "file_reader.h"

typedef struct base_64_decoder {
    file_reader_t file_reader;
    unsigned char* bytes_encoded;
    unsigned char* bytes_decoded;
    FILE * output_file;
} base_64_decoder_t;


int base_64_decoder_init(base_64_decoder_t* self, char* file_to_decode, char* output_file);

int base_64_decoder_decode(base_64_decoder_t* self);

int base_64_decoder_destroy(base_64_decoder_t* self);

#endif