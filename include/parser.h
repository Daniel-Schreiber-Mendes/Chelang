#ifndef PARSER_H
#define PARSER_H
#include <lexer.h>
#include <checl/containers.h>
#include <symbol_table.h>


typedef enum
{
	C_UNIT,
	C_VAR_DEF,
	C_VAR_DEF_INIT,
	C_FUNC_DEF,
	C_FUNC_CALL,
	C_SCOPE,
	C_SCOPE_END,
	C_IF,
	C_ELSE,
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


struct Ast;


typedef struct
{
	enum //OperandType
	{
		OT_NO_OP
	} 
	ot;
	union 
	{
		unsigned int symbol_id;
		unsigned int value;
	};
	struct Ast *scope;
	bool is_num;
}
Operand;


struct Ast
{
	AstType type;
	union
	{
		struct 
		{
			ConstructType ctype;
			Operand o; //temporary that can be assigned to a construct
			Vector symbols; //symbol table
			struct Ast **childs;
			unsigned char child_count, child_cap;
		};
		TokenType ttype;
	};
	struct Ast *parent;
	char *name;
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
bool ast_sort(Ast *ast);
void ast_print(Ast *ast, U8 padding, char const* head);
unsigned int ast_get_index(Ast *ast);
void ast_insert_child(Ast *parent, Ast *child, unsigned int i);
void ast_move(Ast *dest, unsigned int dest_i, Ast *ast, unsigned int i);


bool accept(TokenType t);
bool expect(TokenType t); 
bool check(TokenType t);
Token get_token(I8 d);
void unit(Ast *ast);
Ast parse_tokens(Vector token_array);

void create_symbol_tables(Ast *ast, Ast *last_scope);

#endif