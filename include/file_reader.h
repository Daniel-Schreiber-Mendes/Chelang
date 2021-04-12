#ifndef FILE_READER_H
#define FILE_READERH
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	char *buffer[255];
	unsigned char size;
}
FileArray;

void file_array_create(FileArray *fa);
void file_array_destroy(FileArray *fa);
void read_file(FileArray *fa, char const* path);

#endif