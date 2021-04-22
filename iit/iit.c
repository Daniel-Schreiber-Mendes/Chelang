#include<iit.h>
static Iit iit;


void print_operand(Operand *o)
{
	if (o->ot == OT_NO_OP) 
		return;
	if (o->is_num)
	{
		printf("%i ", o->value);
		return;
	}
	Symbol s = vector_at(&o->scope->symbols, Symbol, o->symbol_id);
	switch (s.symboltype)
	{
		case ST_FUNCTION: 
			printf("	%s:", s.function.name); break;
		case ST_VARIABLE:
			printf("%s", s.variable.name); break;
		case ST_TEMPORARY:
			printf("t%u", s.temporary.id); break;
		case ST_LABEL:
			printf("l%u", s.label.id); break;
	}
}


Iit create_iit(Ast *ast)
{
	create_ii(ast, ast);
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
				break;
			case I_RET_VAL:
				printf("ret ");
				print_operand(&iit.instructions[i].ret_val.o0);
				break;
			case I_JUMP:
				printf("jump ");
				print_operand(&iit.instructions[i].jump.o0);
				print_operand(&iit.instructions[i].jump.o1);
				break;
			case I_FUNC_DEF:
				print_operand(&iit.instructions[i].func_def.o0);
				break;
			default:
				printf("Unknown Instruction: ");
				print_operand(&iit.instructions[i].jump.o0);
		}
		printf("\n");
	}
	return iit;
}


//1. step: create variables
//2. step: create instructions using variables an temporarys 
void create_ii(Ast *ast, Ast *last_scope)
{
	if (ast->type == AT_CONSTRUCT)
	{
		if (ast->ctype == C_UNIT || ast->ctype == C_SCOPE || ast->ctype == C_FUNC_DEF)
		{
			last_scope = ast;
		}
		else if (ast->ctype == C_VAR_DEF || ast->ctype == C_VAR_DEF_INIT)
		{
			create_variable(ast->childs[1], last_scope);
		}
	}



	if (ast->type == AT_CONSTRUCT && ast->ctype == C_FUNC_DEF)
	{
		create_i_func_def(create_function(ast->childs[1], last_scope));
	}
	for (unsigned int i=0; i < ast->child_count; ++i)
	{
		create_ii(ast->childs[i], last_scope);
	}

	if (ast->type == AT_CONSTRUCT)
	{
		if (ast->childs[ast->child_count - 1]->type == AT_TOKEN)
		{
			create_i_assign1(create_temporary(ast, last_scope), create_operand(ast->childs[ast->child_count - 1], last_scope));
		}

		switch (ast->ctype)
		{
			case C_BIN_OP:
				if (ast->childs[1]->ttype == TK_ASSIGN)
				{
					create_i_assign1(ast->o = create_temporary(ast, last_scope), create_operand(ast->childs[2], last_scope));
				}
				else
				{
					create_i_assign2(ast->o = create_temporary(ast, last_scope), create_operand(ast->childs[0], last_scope), ast->childs[1]->name, get_last_operand());
				}
				break;
			case C_VAR_DEF_INIT:
				create_i_assign1(create_operand(ast->childs[1], last_scope), get_last_operand());
				break;
			case C_FUNC_CALL:
				create_i_func_call(create_operand(ast->childs[0], last_scope));
				break;
			case C_RET:
				if (ast->child_count > 0)
					create_i_ret_val(get_last_operand());
				else 
					create_i_ret();
				break;
			default:
				break;
		}
		if (ast->parent)
		{
			if (ast->parent->ctype == C_IF && ast->parent->childs[0] == ast)
			{
				create_i_jump(ast->childs[0]->o, create_label(last_scope)); 
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


void create_i_ret()
{
	iit.instructions[iit.size++].type = I_RET;
}


void create_i_ret_val(Operand o0)
{
	iit.instructions[iit.size++] = (Instruction){I_RET_VAL, .ret_val = (I_ret_val){o0}};
}


void create_i_jump(Operand cond, Operand label)
{
	iit.instructions[iit.size++] = (Instruction){I_JUMP, .jump = (I_jump){cond, label}};
}


void create_i_func_def(Operand o0)
{
	iit.instructions[iit.size++] = (Instruction){I_FUNC_DEF, .func_def = (I_func_def){o0}};
}


Operand create_operand(Ast *ast, Ast *last_scope)
{
	assert(ast && last_scope);
	Operand o;
	o.ot = 10;
	if (ast->type == AT_CONSTRUCT)
	{
		if (ast->ctype == C_BIN_OP)
		{
			printf("YEYE\n");
			o = ast->o;
		}
	}
	else
	{
		if (ast->ttype == TK_INT_LIT)
		{
			o.value = atoi(ast->name);
			o.is_num = 1;
		}
		else if (ast->ttype == TK_ID)
		{
			o.scope = last_scope;
			o.is_num = 0;
			vector_vforeach(&last_scope->symbols, Symbol, s)
			{
				if (s.symboltype == ST_VARIABLE)
				{
					if (!strcmp(s.variable.name, ast->name))
					{
						o.symbol_id = i;
						break;
					}
				}
			}
		}
		else
		{
			assert(false);
		}
	}

	return o;
}


Operand create_temporary(Ast *ast, Ast *last_scope)
{
	unsigned int symbol_id = last_scope->symbols.size;
	vector_push_back(&last_scope->symbols, Symbol, ((Symbol){ST_TEMPORARY, .temporary = (Temporary){symbol_id}}));
	return (Operand){.ot = 10, .symbol_id = symbol_id, .scope = last_scope, .is_num = 0};
}


Operand create_variable(Ast *ast, Ast *last_scope)
{
	unsigned int symbol_id = last_scope->symbols.size;
	vector_push_back(&last_scope->symbols, Symbol, ((Symbol){ST_VARIABLE, .variable = (Variable){ast->name, NULL}}));
	return (Operand){.ot = 10, .symbol_id = symbol_id, .scope = last_scope, .is_num = 0};
}


Operand create_label(Ast *last_scope)
{
	unsigned int symbol_id = last_scope->symbols.size;
	vector_push_back(&last_scope->symbols, Symbol, ((Symbol){ST_LABEL, .label = (Label){symbol_id}}));
	return (Operand){.ot = 10, .symbol_id = symbol_id, .scope = last_scope, .is_num = 0};
}


Operand create_function(Ast *ast, Ast *last_scope)
{
	unsigned int symbol_id = last_scope->symbols.size;
	vector_push_back(&last_scope->symbols, Symbol, ((Symbol){ST_FUNCTION, .function = (Function){ast->name}}));
	return (Operand){.ot = 10, .symbol_id = symbol_id, .scope = last_scope, .is_num = 0};
}



Operand get_last_operand(void)
{
	return iit.instructions[iit.size - 1].func_call.o0;
}