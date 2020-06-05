#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	const char* file_name = argv[1];
	int bytes_number = atoi(argv[2]);

	FILE *fin = fopen(file_name, "r");
	if (NULL == fin)
	{
		perror("cannot open file");
		return -1;
	}
	int data[bytes_number];
	int res = fread(data, sizeof(int), bytes_number, fin);
	printf ("fread result = %i\n", res);
	for(int i = 0; i < res; ++i)
	{
		printf("rand number = %i (0x%x)\n", data[i], data[i]);
	}
	fclose(fin);
}
