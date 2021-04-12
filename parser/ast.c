#include "parser.h"


void ast_create(Ast *ast)
{
	ast->name = "C_UNIT";
	ast->parent = NULL;
	ast->child_count = 0;
	ast->type = AT_CONSTRUCT;
	ast->ctype = C_UNIT;
}


void ast_add_token(Ast *parent, Token t)
{
	assert(parent);
	Ast *child = malloc(sizeof(Ast));
	child->name = t.content;
	child->parent = parent;
	child->type = AT_TOKEN;
	child->ttype = t.type;
	child->child_count = 0;

	parent->childs[parent->child_count++] = child;
}


Ast* ast_add_child(Ast *parent, ConstructType t, char const* name)
{
	assert(parent);
	Ast *child = malloc(sizeof(Ast));
	child->name = name;
	child->parent = parent;
	child->type = AT_CONSTRUCT;
	child->ctype = t;
	child->child_count = 0;

	parent->childs[parent->child_count++] = child;
	return child;
}


Ast* ast_get_parent(Ast* ast)
{
	return ast->parent;
}


void ast_print(Ast *ast, U8 padding, char const* head)
{
	if (ast)
	{
			for (U8 i=0; i < padding - 1; ++i)
			{
				printf("|  ");
			}	
			printf(head);

		if (ast->type == AT_CONSTRUCT)
		{
			printf("%s\n", ast->name);
		}
		else
		{

			printf("%s\n", ast->name);
		}
		{
			for (U8 i=0; i < ast->child_count; ++i)
			{
				ast_print(ast->childs[i], padding+1, i + 1 >=  ast->child_count ? "└──" : "├──");
			}	
		}
	}
}