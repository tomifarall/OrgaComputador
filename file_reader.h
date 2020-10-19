#ifndef _FILE_READER_H_
#define _FILE_READER_H_

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct file_reader {
    FILE* file; 
    int file_size;
    int total_bytes_read;
} file_reader_t;


// Inicializa el file reader. Recibe un nombre de archivo del cual
// leera. En caso de que "file_name" sea nulo se lee la entrada 
// standar

int file_reader_init(file_reader_t* self, const char* file_name);

// Lee el archivo de a chunks de bytes. Estos los storea en buffer.

int file_reader_get_bytes(file_reader_t* self, char* buffer, int chunk);

// Nos indica si el archivo leido esta en EOF.

int file_reader_end_of_file(file_reader_t* self);

// Destruye al file reader, liberando sus recursos.
int file_reader_destroy(file_reader_t* self);

#endif