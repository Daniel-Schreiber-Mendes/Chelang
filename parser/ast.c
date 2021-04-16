#include "parser.h"


void ast_create(Ast *ast)
{
	assert(ast);
	ast->name = "C_UNIT";
	ast->parent = NULL;
	ast->child_count = 0;
	ast->type = AT_CONSTRUCT;
	ast->ctype = C_UNIT;
	vector_construct(&ast->symbols, sizeof(Symbol));
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

	if (child->ctype == C_UNIT || child->ctype == C_SCOPE || child->ctype == C_FUNC_DEF)
	{
		vector_construct(&child->symbols, sizeof(Symbol));
	}

	parent->childs[parent->child_count++] = child;
	return child;
}


Ast* ast_get_penultimate(Ast* ast)
{
	return ast->parent->childs[ast->parent->child_count - 2];
}

/* unsortet ast
C_UNIT: 
└──C_VAR_DEF_INIT
|  ├──Int@
|  ├──arr
|  ├──5
|  └──C_BIN_OP
|  |  ├──*
|  |  ├──1
|  |  └──C_BIN_OP
|  |  |  ├──+
|  |  |  └──1
sortet ast
C_UNIT: 
└──C_VAR_DEF_INIT
|  ├──Int@
|  ├──arr
|  └──C_BIN_OP
|  |  ├──5
|  |  ├──*
|  |  └──C_BIN_OP
|  |  |  ├──1
|  |  |  ├──+
|  |  |  └──1

*/
void ast_sort(Ast *ast)
{
	for (U8 i=0; i < ast->child_count; ++i)
	{
		ast_sort(ast->childs[i]);
	}

	if (ast->type == AT_CONSTRUCT && ast->ctype == C_BIN_OP)
	{
		Ast *left_sibling = ast->parent->childs[ast->parent->child_count - 2];
		left_sibling->parent = ast;
		ast->parent->childs[--ast->parent->child_count - 1] = ast;
		ast->childs[2] = ast->childs[1];
		ast->childs[1] = ast->childs[0];
		ast->childs[0] = left_sibling;
		++ast->child_count;
	}
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
		vector_vforeach(&ast->symbols, Symbol, symbol)
		{
			printf("(%s:%s) ", symbol.type, symbol.name);
		}
	}

	printf("\n");
	for (U8 i=0; i < ast->child_count; ++i)
	{
		ast_print(ast->childs[i], padding+1, i + 1 >=  ast->child_count ? "└──" : "├──");
	}	
}