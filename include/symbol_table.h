#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <parser.h>


typedef struct
{
	char *name;
	unsigned int size;
}
Type;


typedef struct
{
	char *name;
}
Function;


typedef struct
{
	unsigned int id;
}
Temporary;


typedef struct
{
	char *name;
	Type *type;
}
Variable;


typedef struct
{
	unsigned int id;
}
Label;


typedef struct
{
	enum 
	{
		ST_LABEL,
		ST_VARIABLE,
		ST_TEMPORARY,
		ST_FUNCTION,
		ST_TYPE
	}
	symboltype;
	union
	{
		Type type;
		Variable variable;
		Temporary temporary;
		Function function;
		Label label;
	};
}
Symbol;


#endif