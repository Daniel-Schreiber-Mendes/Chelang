#include "lexer.h"
#include "ast.h"


int main(int argc, char **argv)
{
	TokenArray t = lexer_collect_tokens("fnc Int main(){Int x = arr(1, 1, 1, 1)[i[i[i[i[i(1, 1)]]]]];}");
	for (unsigned int i=0; i < t.size; ++i)
	{
		//printf("%s, %u, %li\n", t.tokens[i].content, t.tokens[i].type, t.tokens[i].value);
	}
	create_ast(&t);
}