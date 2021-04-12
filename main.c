#include "lexer.h"
#include "parser.h"
#include "file_reader.h"


int main(int argc, char **argv)
{
	FileArray fa;
	file_array_create(&fa);

	read_file(&fa, "test.che");
	
	Token *t = lexer_collect_tokens(fa.buffer[0]);
	Ast ast = parse_tokens(t);

	file_array_destroy(&fa);
}