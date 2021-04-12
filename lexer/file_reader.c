#include "file_reader.h"


void file_array_create(FileArray *fa)
{
	fa->size = 0;
}


void file_array_destroy(FileArray *fa)
{
	for (unsigned int i=0; i < fa->size; ++i)
	{
		free(fa->buffer[i]);
	}
}


void read_file(FileArray *fa, char const* path)
{
	FILE *f = fopen(path, "r");
	fseek(f, 0, SEEK_END); //move file pointer to end of file
    unsigned int const buffersize = ftell(f); //tell position of pointer which means length of string
    fseek(f, 0, SEEK_SET); //move file pointer back to file begin

    char *const buffer = malloc(buffersize);
    fread(buffer, buffersize, 1, f); //copy memory in file to buffer
    fclose(f);
    buffer[buffersize] = '\0'; //the string has to end with a null terminator, so put it at the end of it
    fa->buffer[fa->size] = buffer;
}