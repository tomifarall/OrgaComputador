#include <stdio.h>
#include "file_reader.h"
#define MASK_ENCODE 0x3F
#define LEN_SRC 3
#define LEN_DEST 4
#define SUCCESS 0
#define ERROR -1


void encode(unsigned char *src, unsigned char *dest);


int main()
{
	unsigned char src[LEN_SRC + 1];
	unsigned char dest[LEN_DEST + 1];
	file_reader_t file_reader;
	FILE *fo;

	fo = fopen("salida.txt","wt");

	file_reader_init(&file_reader, "quijote.txt");
	while (!(file_reader_end_of_file(&file_reader))){
		file_reader_get_bytes(&file_reader, src, LEN_SRC);
		encode(src, dest);
		fputs(dest, fo);
	}

	file_reader_destroy(&file_reader);

	return 0;
}

//juntar con del decode y ponerlo en un tda
void encode(unsigned char *src, unsigned char *dest){
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