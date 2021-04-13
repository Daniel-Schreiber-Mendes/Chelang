#include "parser.h"


void ast_create(Ast *ast)
{
	assert(ast);
	ast->name = "C_UNIT";
	ast->parent = NULL;
	ast->child_count = 0;
	ast->type = AT_CONSTRUCT;
	ast->ctype = C_UNIT;
	ast->symbol_table.size = 0;
}


void ast_add_token(Ast *parent, Token t)
{
	assert(parent);
	Ast *child = malloc(sizeof(Ast));
	child->name = t.content;
	child->parent = parent;
	child->type = AT_TOKEN;
	child->ttype = t.type;

	parent->childs[parent->child_count++] = child;
}


Ast* ast_add_child(Ast *parent, ConstructType t, char const* name)
{
	assert(parent);
	Ast *child = calloc(sizeof(Ast), 1);
	child->name = name;
	child->parent = parent;
	child->type = AT_CONSTRUCT;
	child->ctype = t;

	parent->childs[parent->child_count++] = child;
	return child;
}


void ast_print(Ast *ast, U8 padding, char const* head)
{
	assert(ast);

	for (U8 i=0; i < padding - 1; ++i)
	{
		printf("|  ");
	}	
	printf("%s%s", head, ast->name);
	if (ast->type == AT_CONSTRUCT && (ast->ctype == C_UNIT || ast->ctype == C_SCOPE || ast->ctype == C_FUNC_DEF))
	{
		printf(": ");
		for (unsigned char i=0; i < ast->symbol_table.size; ++i)
		{
			printf("(%s:%s) ", ast->symbol_table.symbols[i].type, ast->symbol_table.symbols[i].name);
		}
	}

	printf("\n");
	for (U8 i=0; i < ast->child_count; ++i)
	{
		ast_print(ast->childs[i], padding+1, i + 1 >=  ast->child_count ? "└──" : "├──");
	}	
}