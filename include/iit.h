#ifndef IIT
#define IIT
#include <parser.h>
#include <symbol_table.h>


//o0 = o1
typedef struct
{
	Operand o0;
	// =
	Operand o1;
}
I_assign1;


// o0 = o1 r_operator o2
typedef struct
{
	Operand o0;
	// =
	Operand o1;
	char *r_operator;
	Operand o2;
}
I_assign2;


typedef struct
{
	Operand o0;
}
I_func_call;


typedef struct
{
	Operand o0; //can be NO_OP if ret does not return a value
}
I_ret;


typedef struct
{
	Operand o0; 
}
I_ret_val;


typedef struct
{
	Operand o0; //condition variable
	Operand o1; //label
}
I_jump;


typedef struct
{
	Operand o0; //label name
}
I_func_def;


typedef struct
{ 
	enum
	{
		I_ASSIGN1,
		I_ASSIGN2,
		I_FUNC_CALL,
		I_RET,
		I_RET_VAL,
		I_JUMP,
		I_FUNC_DEF
	} type;
	union
	{
		I_assign1 assign1;
		I_assign2 assign2;
		I_func_call func_call;
		I_ret ret;
		I_ret_val ret_val;
		I_jump jump;
		I_func_def func_def;
	};
}
Instruction;


typedef struct
{
	Instruction instructions[64];
	unsigned int size;
}
Iit;


Iit create_iit(Ast *ast);
void create_ii(Ast *ast, Ast *last_scope);


void create_i_assign1(Operand o0, Operand o1);
void create_i_assign2(Operand o0, Operand o1, char *r_operator, Operand o2);
void create_i_func_call(Operand o0);
void create_i_ret();
void create_i_ret_val(Operand o0);
void create_i_jump(Operand cond, Operand label);
void create_i_func_def(Operand o0);

Operand create_operand(Ast *ast, Ast *last_scope);
Operand create_label(Ast *last_scope);
Operand create_temporary(Ast *ast, Ast *temporary);
Operand create_variable(Ast *ast, Ast *last_scope);
Operand create_function(Ast *ast, Ast *last_scope);
Operand get_last_operand(void);

#endif