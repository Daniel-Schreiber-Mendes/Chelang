#ifndef AST_H
#define AST_H
#include "lexer.h"

#define FOLLOW_AST_CALLS

//for debugging
extern unsigned char indentation;


#ifdef FOLLOW_AST_CALLS
#define AST_CALL()\
	for (unsigned char i=0; i < indentation; ++i)\
		printf("|");\
	++indentation;\
	printf("\033[0;34m");\
	printf("%s: %s\n", __PRETTY_FUNCTION__, token.content);\
	printf("\033[0m");

#define AST_CALL_END()\
	--indentation;\
	for (unsigned char i=0; i < indentation; ++i)\
		printf("|");\
	printf("\033[0;32m");\
	printf("%s: %s\n", __PRETTY_FUNCTION__, token.content);\
	printf("\033[0m");
#endif

typedef enum
{
	C_STATEMENT,
	C_EXPRESSION,
	C_NO
}
ConstructType;


typedef struct
{
	
}
Ast;

extern Token token;


void expression();
void expression1();
void expression2();
void expression3();
bool accept_literal_expression();
bool accept_unary_expression();

void error(char const* msg);
void forward();
bool accept(TokenType t);
bool expect(TokenType t); 
bool check(TokenType t);
Ast create_ast(TokenArray *tarr);

#endif