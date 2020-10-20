#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include "file_reader.h"

#define SUCCESS 0
#define ERROR -1


/* *****************************************************************
 *                   FILE READER FUNCTIONS                         *
 * *****************************************************************/  


int file_reader_init(file_reader_t* self, const char* file_name) {
    if (!self) return ERROR;
    if (file_name) self->file = fopen(file_name,"rb");
    else 
        self->file = stdin;
    if (!self->file) return ERROR;
    self->total_bytes_read = 0;
    fseek(self->file, 0, SEEK_END);
    self->file_size = ftell(self->file); 
    return SUCCESS;
}

int file_reader_get_bytes(file_reader_t* self, char* buffer, int chunk) {
    if (!self) return ERROR;
    FILE * file = self->file;
    int read_result;
    memset(buffer, 0, chunk);
    fseek(file, self->total_bytes_read, SEEK_SET);
    for (size_t i = 0; i < chunk; i++) {
        read_result = fgetc(file);
        if (read_result == EOF)
            return SUCCESS;
        buffer[i] = read_result;
    }
    buffer[chunk] = '\0';
    self->total_bytes_read += chunk;
    return SUCCESS;
}

int file_reader_destroy(file_reader_t* self)  {
    if (!self) return ERROR;
    fclose(self->file);
    return SUCCESS;
}

int file_reader_end_of_file(file_reader_t* self) {
    if (!self) return ERROR;
    return self->total_bytes_read >= self->file_size;
}
