#include<iit.h>
static Iit iit;

static unsigned int last_var_id, last_label_id;

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
				printf(" %s ", iit.instructions[i].assign2.r_operator);
				print_operand(&iit.instructions[i].assign2.o2);
				break;
			case I_FUNC_CALL:
				printf("call ");
				print_operand(&iit.instructions[i].func_call.o0);
				break;
			case I_RET:
				printf("ret ");
				if (iit.instructions[i].ret.o0.ot != OT_NO_OP)
					print_operand(&iit.instructions[i].ret.o0);
				break;
			case I_JUMP:
				printf("jump ");
				print_operand(&iit.instructions[i].jump.o0);
				printf(" l%u", iit.instructions[i].jump.o1.label_id);
				break;
			case I_FUNC_DEF:
				printf("func_def: l%u", iit.instructions[i].func_def.o0.label_id);
				break;
			default:
				printf("Unknown Instruction: ");
				print_operand(&iit.instructions[i].jump.o0);
		}
		printf("\n");
	}
	return iit;
}


void create_ii(Ast *ast)
{
	if (ast->type == AT_CONSTRUCT && ast->ctype == C_FUNC_DEF)
	{
		create_i_func_def(create_label(ast));
	}
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
			case C_RET:
				if (ast->child_count > 0)
					create_i_ret(get_last_operand());
				else 
					create_i_ret(NO_OPERAND);
				break;
			default:
				break;
		}
		if (ast->parent)
		{
			if (ast->parent->ctype == C_IF && ast->parent->childs[0] == ast)
			{
				create_i_jump(ast->childs[0]->o, create_label(LOCAL_LABEL)); 
			}
			//todo: implement else
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


void create_i_ret(Operand o0)
{
	iit.instructions[iit.size++] = (Instruction){I_RET, .ret = (I_ret){o0}};
}


void create_i_jump(Operand cond, Operand label)
{
	iit.instructions[iit.size++] = (Instruction){I_JUMP, .jump = (I_jump){cond, label}};
}


void create_i_func_def(Operand label)
{
	iit.instructions[iit.size++] = (Instruction){I_FUNC_DEF, .func_def = (I_func_def){label}};
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


unsigned int get_label_id(Ast *ast)
{
	return last_label_id++;
}


Operand create_temporary(void)
{
	return (Operand){{get_var_id(NULL)}, "Unknown Type", OT_VAR};
}


Operand create_variable(Ast *ast)
{
	
}


Operand create_label(Ast *ast)
{
	if (ast)
	{
		unsigned int label_id = get_label_id(ast);
		return (Operand){{label_id}, "No Type", OT_LABEL};
	}
	else
	{
		return (Operand){{get_label_id(ast)}, "No Type", OT_LABEL};
	}
}


Operand get_last_operand(void)
{
	return iit.instructions[iit.size - 1].func_call.o0;
}