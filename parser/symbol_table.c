#include<parser.h>


void create_symbol_tables(Ast *ast, Ast *last_scope)
{
	//if theres a lower scope than the last one, add all symbols from now to that ast until a lower one is found
	if (ast->type == AT_CONSTRUCT && (ast->ctype == C_UNIT || ast->ctype == C_SCOPE || ast->ctype == C_FUNC_DEF))
	{
		last_scope = ast;
	}

	if (ast->type == AT_CONSTRUCT)
	{
		if (ast->ctype == C_VAR_DEF || ast->ctype == C_VAR_DEF_INIT)
		{
			vector_push_back(&last_scope->symbols, Symbol, ((Symbol){0, ast->childs[1]->name, ast->childs[0]->name}));
		}
		else if (ast->ctype == C_FUNC_DEF)
		{
			for (U8 i=0; i < ast->child_count; ++i)
			{
				if (i % 2 == 1) //every second token is an identifier
				{
					vector_push_back(&last_scope->symbols, Symbol, ((Symbol){0, ast->childs[i]->name, ast->childs[i - 1]->name}));
				}
			}
		}
		else if (ast->ctype == C_STRUCT_DEF)
		{
			vector_push_back(&last_scope->symbols, Symbol, ((Symbol){0, ast->childs[0]->name, "Struct"}));
		}
	}

	for (U8 i=0; i < ast->child_count; ++i)
	{
		create_symbol_tables(ast->childs[i], last_scope);
	}
}