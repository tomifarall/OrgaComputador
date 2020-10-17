#include <stdio.h>
#define MASK_ENCODE 0x3F
#define LEN_SRC 3
#define LEN_DEST 4

void encode(unsigned char *src, unsigned char *dest);


int main()
{
	unsigned char src[LEN_SRC] = "En ";
	unsigned char dest[LEN_DEST];

	encode(src, dest);
	printf("%d %d %d %d\n", dest[0], dest[1], dest[2], dest[3]);

	return 0;
}

void encode(unsigned char *src, unsigned char *dest){
	for (int i = 0; i < LEN_DEST; i++)
	{
		dest[LEN_DEST-(i+1)] = src[2] & MASK_ENCODE;
		for (int j = 0; j < LEN_SRC-1; j++)
		{
			src[LEN_SRC-(j+1)] >>= 6;
			src[LEN_SRC-(j+1)] |= ((src[LEN_SRC-(j+2)] & MASK_ENCODE) << 2);
		}
		src[0] >>= 6;
	}
}