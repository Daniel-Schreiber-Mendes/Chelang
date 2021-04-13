#include "parser.h"


static Token *tokens;
static U16 token_index;
Token token;

bool accept(TokenType t) 
{
    if (token.type == t) 
    {
        token = tokens[++token_index];
        return 1;
    }
    return 0;
}

bool expect(TokenType t) 
{
    if (accept(t))
        return 1;
    printf("Error: expected token \"%u\" but found \"%s\"\n", t, token.content);
	assert(0);
    return 0;
}


bool check(TokenType t)
{
    return token.type == t;
}


Token get_token(I8 d)
{
	return tokens[token_index + d];
}


void unit(Ast *ast)
{
	while (!accept(TK_EOF))
	{
		var_fnc_type_def(ast);
	}
}


Ast parse_tokens(Token *token_array)
{
	tokens = token_array;
	token = tokens[0];

	Ast ast;
	ast_create(&ast);
	unit(&ast);

	return ast;
}