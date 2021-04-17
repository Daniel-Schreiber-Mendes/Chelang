#ifndef PARSER_H
#define PARSER_H
#include <lexer.h>
#include <checl/containers.h>


typedef enum
{
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
	C_GROUP,
	C_STRUCT_DEF
}
ConstructType;


typedef enum
{
	AT_CONSTRUCT,
	AT_TOKEN
}
AstType;


typedef struct
{
	unsigned int size;
	char *name;
	char *type;
}
Symbol;



typedef struct
{
	union
	{
		int value;
		unsigned int var_id;
	};
	char *type; //type of the operand, can be null if it is not known
	enum //OperandType
	{
		OT_NO_OP,
		OT_NUM,
		OT_VAR	
	} 
	ot;
}
Operand;


struct Ast
{
	AstType type;
	union
	{
		ConstructType ctype;
		TokenType ttype;
	};
	char *name;
	struct Ast *parent;
	struct Ast *childs[16];
	U8 child_count;
	Vector symbols; //symbol table
	Operand o; //temporary
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
bool accept_var_fnc_type_def(Ast *ast);
void var_fnc_type_def(Ast *ast);

void ast_create(Ast *ast);
Ast* ast_add_child(Ast *parent, ConstructType t, char const* name);
void ast_add_token(Ast *parent, Token t);
Ast* ast_get_penultimate(Ast* ast);
bool ast_sort(Ast *ast);
void ast_print(Ast *ast, U8 padding, char const* head);
Ast* ast_get_left_sibling(Ast *ast);
unsigned int ast_get_index(Ast *ast);
void ast_insert_child(Ast *parent, Ast *child, unsigned int i);
void ast_move(Ast *dest, unsigned int dest_i, Ast *ast, unsigned int i);


bool accept(TokenType t);
bool expect(TokenType t); 
bool check(TokenType t);
Token get_token(I8 d);
void unit(Ast *ast);
Ast parse_tokens(Token *token_array);

void create_symbol_tables(Ast *ast, Ast *last_scope);

#endif