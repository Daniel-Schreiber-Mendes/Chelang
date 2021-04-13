#include "parser.h"


/*
U: &
└: @
└: !
*/
bool accept_unary()
{
	if (accept(TK_NOT)) return 1;
	if (accept(TK_AMPERCENT)) return 1;
	if (accept(TK_AT)) return 1;
	return 0;
}


bool accept_literal()
{
	if (accept(TK_ID)) return 1;
	if (accept(TK_STRING)) return 1;
	if (accept(TK_INT_LIT)) return 1;
	return 0;
}


bool accept_binary()
{
	if (accept(TK_ASSIGN)) return 1;
	if (accept(TK_PLUS)) return 1;
	if (accept(TK_MINUS)) return 1;
	if (accept(TK_STAR)) return 1;
	if (accept(TK_DIV)) return 1;
	if (accept(TK_ARROW)) return 1;
	if (accept(TK_POINT)) return 1;
	if (accept(TK_LESSTHAN)) return 1;
	if (accept(TK_MORETHAN)) return 1;
	return 0;
}


/*
E: lit C
└: ( E ) C
└: U E C
*/
void expression(Ast *ast)
{
	if (accept_literal())
	{
		if (strcmp(token.content, "(")) //if not function call, because then the token comes after the C_FUNC_CALL token
		{
			ast_add_token(ast, get_token(-1));
		}

		expression1(ast);
	}
	else if (accept(TK_OP))
	{
		ast = ast_add_child(ast, C_GROUP, "C_GROUP");
		expression(ast);
		expect(TK_CP);
		expression1(ast);
	}
	else if (accept_unary())
	{
		ast_add_token(ast, get_token(-1));
		expression(ast);
		expression1(ast);
	}
	else
	{
		printf("Error no expression found: %s\n", token.content);
	}
}

/*
C: [ E ] C
└: ( A ) C
└: O E C
└:
*/
void expression1(Ast *ast)
{
	if (accept(TK_OSB))
	{
		ast = ast_add_child(ast, C_ARR_INDX, "C_ARR_INDX");
		expression(ast);
		expect(TK_CSB);
		expression1(ast);
	}
	else if (accept(TK_OP))
	{
		ast = ast_add_child(ast, C_FUNC_CALL, "C_FUNC_CALL");
		ast_add_token(ast, get_token(-2));
		expression2(ast);
		expect(TK_CP);
		expression1(ast);
	}
	else if (accept_binary())
	{
		ast = ast_add_child(ast, C_BIN_OP, "C_BIN_OP");
		ast_add_token(ast, get_token(-1));
		expression(ast);//
		expression1(ast);
	}
}

/*
A:
└: B
*/
void expression2(Ast *ast)
{
	accept_expression3(ast);
}

/*
B: lit C D
└: ( E ) C D
└: U E C D
*/
void expression3(Ast *ast)
{
	if (accept_literal())
	{
		ast_add_token(ast, get_token(-1));
		expression1(ast);
		expression4(ast);
	}
	else if (accept(TK_OP))
	{
		ast = ast_add_child(ast, C_GROUP, "C_GROUP");
		ast_add_token(ast, get_token(-2));
		expression(ast);
		expect(TK_CP);
		expression1(ast);
		expression4(ast);
	}
	else if (accept_unary())
	{
		ast_add_token(ast, get_token(-1));
		expression(ast);
		expression1(ast);
		expression4(ast);
	}
	else
	{
		printf("Error no expression3 found, %s\n", token.content);	
	}
}


/*
B: lit C D
└: ( E ) C D
└: U E C D
*/
bool accept_expression3(Ast *ast)
{
	if (accept_literal())
	{
		expression1(ast);
		expression4(ast);
	}
	else if (accept(TK_OP))
	{
		expression(ast);
		expect(TK_CP);
		expression1(ast);
		expression4(ast);
	}
	else if (accept_unary())
	{
		ast_add_token(ast, get_token(-1));
		expression(ast);
		expression1(ast);
		expression4(ast);
	}
	else
	{
		return 0;
	}
	return 1;
}

/*
D: , B D
└:
*/
void expression4(Ast *ast)
{
	if (accept(TK_COMMA))
	{
		expression3(ast);
		expression4(ast);
	}
}