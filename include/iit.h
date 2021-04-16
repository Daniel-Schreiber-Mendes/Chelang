#ifndef IIT
#define IIT
#include<parser.h>


typedef enum
{
	OT_NO_OP,
	OT_NUM,
	OT_VAR
}
OperandType;


typedef enum
{
	IIT_ASSIGN_1,
	IIT_ASSIGN_2,
	IIT_FUNC_CALL
}
IIType;


typedef struct
{
	char *type; //type of the operand, can be null if it is not known
	union
	{
		int value;
		int var_id;
	};
	OperandType ot;
}
Operand;


typedef struct
{ 
	char *left_operator;
	char *right_operator; //optional
	Operand o0, o1, o2;
	IIType type;
}
Iinstruction;


typedef struct
{
	Iinstruction instructions[64];
	unsigned int size;
}
Iit;


#define NO_OPERAND ((Operand){0, {0}, 0})


Iit create_iit(Ast *ast);
void create_ii(Ast *ast);

void create_iinstruction(char *left_operator, char *right_operator, Operand o0, Operand o1, Operand o2);
Operand create_operand(Ast *ast, bool make_temporary);
unsigned int get_var_id(Ast *ast);
Operand create_temporary(void);

#endif