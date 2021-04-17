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
C_UNIT (1): 
└──C_VAR_DEF_INIT (3)
|  ├──Int@ (0)
|  ├──arr (0)
|  └──C_GROUP (3)
|  |  ├──5 (0)
|  |  ├──C_BIN_OP (2)
|  |  |  ├──* (0)
|  |  |  └──C_GROUP (2)
|  |  |  |  ├──1 (0)
|  |  |  |  └──C_BIN_OP (2)
|  |  |  |  |  ├──+ (0)
|  |  |  |  |  └──1 (0)
|  |  └──C_BIN_OP (2)
|  |  |  ├──* (0)
|  |  |  └──C_GROUP (2)
|  |  |  |  ├──3 (0)
|  |  |  |  └──C_BIN_OP (2)
|  |  |  |  |  ├──+ (0)
|  |  |  |  |  └──4 (0)
sortet ast
C_UNIT (1): 
└──C_VAR_DEF_INIT (3)
|  ├──Int@ (0)
|  ├──arr (0)
|  └──C_GROUP (1)
|  |  └──C_BIN_OP (3)
|  |  |  ├──C_BIN_OP (3)
|  |  |  |  ├──5 (0)
|  |  |  |  ├──* (0)
|  |  |  |  └──C_GROUP (1)
|  |  |  |  |  └──C_BIN_OP (3)
|  |  |  |  |  |  ├──1 (0)
|  |  |  |  |  |  ├──+ (0)
|  |  |  |  |  |  └──1 (0)
|  |  |  ├──* (0)
|  |  |  └──C_GROUP (1)
|  |  |  |  └──C_BIN_OP (3)
|  |  |  |  |  ├──3 (0)
|  |  |  |  |  ├──+ (0)
|  |  |  |  |  └──4 (0)


*/
bool ast_sort(Ast *ast)
{
	for (U8 i=0; i < ast->child_count; ++i)
	{
		if (ast_sort(ast->childs[i])) //if a child got removed
		{
			if (i < ast->child_count)
				ast_sort(ast->childs[i]);
		}
	}
	if (ast->type == AT_CONSTRUCT && ast->ctype == C_BIN_OP)
	{
		Ast *left_sibling = ast_get_left_sibling(ast);
		unsigned int i = ast_get_index(ast);
		ast->parent->childs[i - 1] = ast;
		ast->parent->childs[i] = ast->parent->childs[i + 1];
		ast->parent->child_count--;
//Problem: if parent child count is decreased but rightmost sibling has not been visitet it is ignored
		ast_insert_child(ast, left_sibling, 0);
		return true;
	}
	return false;
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



Ast* ast_get_left_sibling(Ast *ast)
{
	for (unsigned int i=0; i < ast->parent->child_count; ++i)
	{
		if (ast->parent->childs[i] == ast)
		{
			return ast->parent->childs[i - 1];
		}
	}
	assert(false);
	return NULL;
}


unsigned int ast_get_index(Ast *ast)
{
	for (unsigned int i=0; i < ast->parent->child_count; ++i)
	{
		if (ast->parent->childs[i] == ast)
		{
			return i;
		}
	}
	assert(false);
	return 0;
}


void ast_insert_child(Ast *parent, Ast *child, unsigned int i)
{
	memmove(&parent->childs[parent->child_count - 1 - i], &parent->childs[i], (parent->child_count - i) * sizeof(Ast*));
	parent->childs[i] = child;
	child->parent = parent;
	++parent->child_count;
}