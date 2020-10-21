#include "base_64_decoder.h"

#define BYTES_CHUNK 4
#define ERROR -1
#define SUCCESS 0


/* *****************************************************************
 *                   AUXILIARY FUNCTIONS                           *
 * *****************************************************************/ 

const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
    59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
	43, 44, 45, 46, 47, 48, 49, 50, 51 };

// void b64_generate_decode_table()
// {
// 	int    inv[80];
// 	size_t i;

// 	memset(inv, -1, sizeof(inv));
// 	for (i=0; i<sizeof(b64chars)-1; i++) {
// 		inv[b64chars[i]-43] = i;
// 	}
// }

size_t b64_decoded_size(const char *in)
{
	size_t len;
	size_t ret;
	size_t i;

	if (in == NULL)
		return 0;

	len = strlen(in);
	ret = (len / 4) * 3;

	for (i=len; i >=0; i--) {
		if (in[i] == '=') {
			ret--;
		} else {
			break;
		}
	}

	return ret;
}

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

int b64_decode(const char *in, unsigned char *out, size_t outlen)
{
	size_t len;
	size_t i;
	size_t j;
	int    v;

	if (in == NULL || out == NULL)
		return 0;

	len = strlen(in);
	if (outlen < b64_decoded_size(in) || len % 4 != 0)
		return 0;

	for (i=0; i<len; i++) {
		if (!is_valid_b64_char(in[i])) {
			return 0;
		}
	}

	for (i=0, j=0; i<len; i+=4, j+=3) {
		v = b64invs[in[i]-43];
		v = (v << 6) | b64invs[in[i+1]-43];
		v = in[i+2]=='=' ? v << 6 : (v << 6) | b64invs[in[i+2]-43];
		v = in[i+3]=='=' ? v << 6 : (v << 6) | b64invs[in[i+3]-43];

		out[j] = (v >> 16) & 0xFF;
		if (in[i+2] != '=')
			out[j+1] = (v >> 8) & 0xFF;
		if (in[i+3] != '=')
			out[j+2] = v & 0xFF;
	}

	return 1;
}

/* *****************************************************************
 *               BASE 64 DECODER FUNCTIONS                         *
 * *****************************************************************/

int base_64_decoder_init(base_64_decoder_t* self, char* file_to_decode) {
    if (!self) return ERROR;
    if (file_reader_init(&self->file_reader, file_to_decode) != SUCCESS)
        return ERROR;
    self->bytes_encoded = malloc(sizeof(char) * BYTES_CHUNK);
    if (!self->bytes_encoded) {
        file_reader_destroy(&self->file_reader);
        return ERROR;
    }
    self->bytes_decoded = malloc(sizeof(char) * 3);
    if (!self->bytes_decoded) {
        free(self->bytes_encoded);
        file_reader_destroy(&self->file_reader);
        return ERROR;
    }
    memset(self->bytes_encoded, 0, BYTES_CHUNK);
    memset(self->bytes_decoded, 0, 3);
    self->total_bytes_read = 0;
    return SUCCESS;
}


int base_64_decoder_decode(base_64_decoder_t* self) {
    if (!self) return ERROR;

    while (!file_reader_end_of_file(&self->file_reader)) {
        file_reader_get_bytes(&self->file_reader, self->bytes_encoded, BYTES_CHUNK);
        for (int i = 0; i < BYTES_CHUNK; i++) {
            if (!is_valid_b64_char(self->bytes_encoded[i])) 
                return ERROR;
        }
        b64_decode(self->bytes_encoded, self->bytes_decoded, 3);
        printf("%s\n", self->bytes_decoded);
    }

    return SUCCESS;
}


int base_64_decoder_destroy(base_64_decoder_t* self) {
    if (!self) return ERROR;
    file_reader_destroy(&self->file_reader);
    free(self->bytes_encoded);
    free(self->bytes_decoded);
    return SUCCESS;
}



int main(int argc, char const *argv[])
{
    base_64_decoder_t base_64_decoder;

    base_64_decoder_init(&base_64_decoder, "test.txt");

    base_64_decoder_decode(&base_64_decoder);

    base_64_decoder_destroy(&base_64_decoder);

    return 0;
}