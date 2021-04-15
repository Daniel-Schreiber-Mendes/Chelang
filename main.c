#include "lexer.h"
#include "parser.h"
#include "file_reader.h"
#include<iit.h>

int main(int argc, char **argv)
{
	FileArray fa;
	file_array_create(&fa);

	read_file(&fa, "test.che");
	
	Ast ast = parse_tokens(lexer_collect_tokens(fa.buffer[0]));
	create_symbol_tables(&ast, NULL);
	create_iit(&ast);
	ast_print(&ast, 0, "");

	file_array_destroy(&fa);
}