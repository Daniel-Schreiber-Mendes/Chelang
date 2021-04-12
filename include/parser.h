#ifndef PARSER_H
#define PARSER_H
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
#ifndef FOLLOW_AST_CALLS
#define AST_CALL()
#define AST_CALL_END()
#endif

typedef enum
{
	C_STATEMENT,
	C_EXPR,
	C_UNIT,
	C_VAR_DEF,
	C_VAR_DEF_INIT,
	C_FUNC_DEF,
	C_FUNC_CALL,
	C_SCOPE,
	C_IF,
	C_WHILE,
	C_RET,
	C_ARR_INDX,
	C_BIN_OP,
	C_GROUP
}
ConstructType;


typedef enum
{
	AT_CONSTRUCT,
	AT_TOKEN
}
AstType;


struct Ast
{
	AstType type;
	union
	{
		ConstructType ctype;
		TokenType ttype;
	};
	char const *name;
	struct Ast *parent;
	struct Ast *childs[16];
	U8 child_count;
};

typedef struct Ast Ast;

extern Token token;


void expression(Ast *ast);
void expression1(Ast *ast);
void expression2(Ast *ast);
void expression3(Ast *ast);
bool accept_expression3(Ast *ast);
void expression4(Ast *ast);
bool accept_literal(void);
bool accept_unary(void);
bool accept_binary(void);

void statement(Ast *ast);
bool accept_var_or_fnc_def(Ast *ast);
void var_or_fnc_def(Ast *ast);

void ast_add_token(Ast *parent, Token t);
void ast_create(Ast *ast);
Ast* ast_add_child(Ast *parent, ConstructType t, char const* name);
Ast* ast_get_parent(Ast* ast);
void ast_print(Ast *ast, U8 padding, char const* head);

void forward(void);
bool accept(TokenType t);
bool expect(TokenType t); 
bool check(TokenType t);
Token get_token(I8 d);
bool check_ahead(TokenType t, U8 d);
void unit(Ast *ast);
Ast parse_tokens(Token *token_array);

#endif