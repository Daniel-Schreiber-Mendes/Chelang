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
			create_iinstruction("=", "NO_OP", create_operand(ast, true), create_operand(ast->childs[ast->child_count - 1], false), NO_OPERAND);
		}
		if (ast->ctype == C_BIN_OP)
		{
			create_iinstruction("=", ast->childs[0]->name, create_operand(ast_get_penultimate(ast), true), create_operand(ast->parent, false), iit.instructions[iit.size - 1].o0);
			printf("%s\n", ast_get_penultimate(ast)->name);
		}
	}
}


void create_iinstruction(char *left_operator, char *right_operator, Operand o0, Operand o1, Operand o2)
{
	iit.instructions[iit.size++] = (Iinstruction){left_operator, right_operator, o0, o1, o2};
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
	return o;
}


unsigned int get_var_id(Ast *ast)
{
	return last_var_id++;
}