#include "ast.h"

//only for debugging
unsigned char indentation;


static TokenArray *ta;
Token token;
static unsigned int ti;
static Ast ast;
static ConstructType construct;


void error(char const* msg)
{
	printf("%s\nToken: %s\n", msg, token.content);
	exit(0);
}


void forward()
{
	token = ta->tokens[++ti];
}


bool accept(TokenType t) 
{
    if (token.type == t) 
    {
    	printf("%s\n", token.content);
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


void statement(void)
{
    if (accept(TK_OB)) 
    {	
        if (accept(TK_CB)) return; //if statement is empty

        while (!check(TK_CB))
        {
            statement();
        }

        expect(TK_CB);
    } 
    else if(accept(TK_TYPE))
    {
    	expect(TK_ID);
    	if (accept(TK_ASSIGN))
    	{
    		expression();
    	}
    	expect(TK_SCLN);
    }
    else if(accept(TK_SCLN))
    {

    }
    else 
    {
        expression();
        expect(TK_SCLN);
    }
}


void unit(void)
{
	while (accept(TK_FNC))
	{
		expect(TK_TYPE);
		expect(TK_ID);
		expect(TK_OP);
		expect(TK_CP);
		statement();
	}
	expect(TK_EOF);
}


Ast create_ast(TokenArray *tarr)
{
	indentation = 0;
	ta = tarr;
	token = ta->tokens[0];

	unit();

	return ast;
}