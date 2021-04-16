#include<iit.h>
static Iit iit;

static unsigned int last_var_id;

void print_operand(Operand *o)
{
	if (o->ot == OT_VAR)
		printf("t");

	printf("%i ", o->value);
}


Iit create_iit(Ast *ast)
{
	create_ii(ast);
	for (unsigned int i=0; i < iit.size; ++i)
	{
		print_operand(&iit.instructions[i].o0);
		printf("%s ", iit.instructions[i].left_operator);
		print_operand(&iit.instructions[i].o1);
		if (!iit.instructions[i].right_operator)
		{
			printf("\n");			
			continue;
		}
		printf("%s ", iit.instructions[i].right_operator);
		print_operand(&iit.instructions[i].o2);
		printf("\n");
	}
	return iit;
}


void create_ii(Ast *ast)
{
	for (unsigned int i=0; i < ast->child_count; ++i)
	{
		create_ii(ast->childs[i]);
	}

	if (ast->type == AT_CONSTRUCT)
	{
		if (ast->childs[ast->child_count - 1]->type == AT_TOKEN)
		{
			create_iinstruction("=", NULL, create_temporary(), create_operand(ast->childs[ast->child_count - 1], false), NO_OPERAND);
		}
		if (ast->ctype == C_BIN_OP)
		{
			if (ast->childs[1]->ttype == TK_ASSIGN)
			{
				create_iinstruction("=", NULL, create_temporary(), create_operand(ast->childs[2], false), NO_OPERAND);
			}
			else
			{
				create_iinstruction("=", ast->childs[1]->name, create_temporary(), create_operand(ast->childs[0], false), iit.instructions[iit.size - 1].o0);
			}
		}
		else if (ast->ctype == C_VAR_DEF_INIT)
		{
			create_iinstruction("=", NULL, create_temporary(), iit.instructions[iit.size - 1].o0, NO_OPERAND);
		}
		else if (ast->ctype == C_FUNC_CALL)
		{
			create_iinstruction("call", NULL, create_operand(ast->childs[0], false), NO_OPERAND, NO_OPERAND);
		}
	}
}


void create_iinstruction(char *left_operator, char *right_operator, Operand o0, Operand o1, Operand o2)
{
	Iinstruction ii = (Iinstruction){left_operator, right_operator, o0, o1, o2};
	if (!strcmp("call", left_operator))
	{
		ii.type = IIT_FUNC_CALL;
	}
	iit.instructions[iit.size++] = ii;
}


Operand create_operand(Ast *ast, bool make_temporary)
{
	Operand o;
	o.type = "Unknown Type";
	if (make_temporary)
	{
		o.var_id = get_var_id(ast);
		o.ot = OT_VAR;
	}
	else if (ast->ttype == TK_INT_LIT)
	{
		o.value = atoi(ast->name);
		o.ot = OT_NUM;
	}
	else if (ast->ttype == TK_ID)
	{
		o.var_id = 10;
		o.ot = OT_VAR;
	}
	return o;
}


unsigned int get_var_id(Ast *ast)
{
	return last_var_id++;
}


Operand create_temporary(void)
{
	return (Operand){"Unknown Type", get_var_id(NULL), OT_VAR};
}