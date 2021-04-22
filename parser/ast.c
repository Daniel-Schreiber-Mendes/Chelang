#include "parser.h"


void ast_create(Ast *ast)
{
	assert(ast);
	ast->name = "C_UNIT";
	ast->parent = NULL;
	ast->child_count = 0;
	ast->type = AT_CONSTRUCT;
	ast->ctype = C_UNIT;
	ast->child_cap = 1;
	ast->childs = malloc(sizeof(Ast));
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
	child->child_cap = 1;
	child->childs = malloc(sizeof(Ast));

	if (child->ctype == C_UNIT || child->ctype == C_SCOPE || child->ctype == C_FUNC_DEF)
	{
		vector_construct(&child->symbols, sizeof(Symbol));
	}

	if (parent->child_count + 1 > parent->child_cap)
		parent->childs = realloc(parent->childs, parent->child_cap * 2);

	parent->childs[parent->child_count++] = child;
	return child;
}


bool ast_sort(Ast *ast)
{
	assert(ast);
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
		unsigned int i = ast_get_index(ast);
		Ast *left_sibling = ast->parent->childs[i - 1];
		ast->parent->childs[i - 1] = ast;
		ast->parent->childs[i] = ast->parent->childs[i + 1];
		ast->parent->child_count--;
		ast_insert_child(ast, left_sibling, 0);
		return true;
	}
	return false;
}


unsigned int ast_get_index(Ast *ast)
{
	assert(ast);
	for (unsigned int i=0; i < ast->parent->child_count; ++i)
	{
		if (ast->parent->childs[i] == ast)
			return i;
	}
	assert(false);
	return 0;
}


void ast_insert_child(Ast *parent, Ast *child, unsigned int i)
{
	if (parent->child_count + 1 > parent->child_cap)
		parent->childs = realloc(parent->childs, parent->child_cap * 2);

	memmove(&parent->childs[parent->child_count - 1 - i], &parent->childs[i], (parent->child_count - i) * sizeof(Ast*));
	parent->childs[i] = child;
	child->parent = parent;
	++parent->child_count;
}


void ast_print(Ast *ast, U8 padding, char const* head)
{
	assert(ast);

	for (U8 i=0; i < padding - 1; ++i)
	{
		printf("|  ");
	}	
	printf("%s%s", head, ast->name);

	printf("\n");
	for (U8 i=0; i < ast->child_count; ++i)
	{
		ast_print(ast->childs[i], padding+1, i + 1 >=  ast->child_count ? "└──" : "├──");
	}	
}