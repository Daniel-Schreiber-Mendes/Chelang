#include<iit.h>
static Iit iit;

static unsigned int last_var_id;

void print_operand(Operand *o)
{
	if (o->ot == OT_VAR)
	{
		printf("t%u", o->var_id);
	}
	else
		printf("%i ", o->value);
}


Iit create_iit(Ast *ast)
{
	create_ii(ast);
	for (unsigned int i=0; i < iit.size; ++i)
	{
		switch (iit.instructions[i].type)
		{
			case I_ASSIGN1:
				print_operand(&iit.instructions[i].assign1.o0);
				printf(" = ");
				print_operand(&iit.instructions[i].assign1.o1);
				break;
			case I_ASSIGN2:
				print_operand(&iit.instructions[i].assign2.o0);
				printf(" = ");
				print_operand(&iit.instructions[i].assign2.o1);
				printf("%s ", iit.instructions[i].assign2.r_operator);
				print_operand(&iit.instructions[i].assign2.o2);
				break;
			case I_FUNC_CALL:
				printf("call ");
				print_operand(&iit.instructions[i].func_call.o0);
				break;
		}
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
			create_i_assign1(create_temporary(), create_operand(ast->childs[ast->child_count - 1]));
		}

		switch (ast->ctype)
		{
			case C_BIN_OP:
				if (ast->childs[1]->ttype == TK_ASSIGN)
				{
					create_i_assign1(ast->o = create_temporary(), create_operand(ast->childs[2]));
				}
				else
				{
					create_i_assign2(ast->o = create_temporary(), create_operand(ast->childs[0]), ast->childs[1]->name, get_last_operand());
				}
				break;
			case C_VAR_DEF_INIT:
				create_i_assign1(create_temporary(), get_last_operand());
				break;
			case C_FUNC_CALL:
				create_i_func_call(create_operand(ast->childs[0]));
				break;
			default:
				break;
		}
	}
}


void create_i_assign1(Operand o0, Operand o1)
{
	iit.instructions[iit.size++] = (Instruction){I_ASSIGN1, .assign1 = (I_assign1){o0, o1}};
}


void create_i_assign2(Operand o0, Operand o1, char *r_operator, Operand o2)
{
	iit.instructions[iit.size++] = (Instruction){I_ASSIGN2, .assign2 = (I_assign2){o0, o1, r_operator, o2}};
}


void create_i_func_call(Operand o0)
{
	iit.instructions[iit.size++] = (Instruction){I_FUNC_CALL, .func_call = (I_func_call){o0}};
}


Operand create_operand(Ast *ast)
{
	Operand o;
	o.type = "Unknown Type";
	o.var_id = 999;
	if (ast->type == AT_CONSTRUCT)
	{
		if (ast->ctype == C_BIN_OP)
		{
			o = ast->o;
		}
	}
	else
	{
		if (ast->ttype == TK_INT_LIT)
		{
			o.value = atoi(ast->name);
			o.ot = OT_NUM;	
		}
		else if (ast->ttype == TK_ID)
		{
			o.var_id = get_var_id(ast);
			o.ot = OT_VAR;
		}
		else
		{
			assert(false);
		}
	}
	return o;
}


unsigned int get_var_id(Ast *ast)
{
	return last_var_id++;
}


Operand create_temporary(void)
{
	return (Operand){{get_var_id(NULL)}, "Unknown Type", OT_VAR};
}


Operand get_last_operand(void)
{
	return iit.instructions[iit.size - 1].func_call.o0;
}