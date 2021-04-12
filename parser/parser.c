#include "parser.h"

//only for debugging
unsigned char indentation;

static Token *tokens;
static U16 token_index;
Token token;


void forward()
{
	token = tokens[++token_index];
}


bool accept(TokenType t) 
{
    if (token.type == t) 
    {
    	//printf("%s\n", token.content);
        forward();
        return 1;
    }
    return 0;
}

bool expect(TokenType t) 
{
    if (accept(t))
        return 1;
    printf("Error: expected \"%u\" but found \"%s\"\n", t, token.content);
	exit(0);
    return 0;
}


bool check(TokenType t)
{
    return token.type == t;
}


bool check_ahead(TokenType t, U8 d)
{
    return tokens[token_index + d].type == t;
}


Token get_token(I8 d)
{
	return tokens[token_index + d];
}


void unit(Ast *ast)
{
	while (!accept(TK_EOF))
	{
		var_or_fnc_def(ast);
	}
}


Ast parse_tokens(Token *token_array)
{
	indentation = 0;
	tokens = token_array;
	token = tokens[0];

	Ast ast;
	ast_create(&ast);
	unit(&ast);

	ast_print(&ast, 0, "");
	return ast;
}