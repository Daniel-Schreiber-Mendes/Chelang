#include "ast.h"


/*
U: &
└: @
└: ->
*/
bool accept_unary_expression()
{
	if (accept(TK_NOT))
	{
		
	}
	else if (accept(TK_AMPERCENT))
	{
		
	}
	else if (accept(TK_AT))
	{
		
	}
	else
	{
		return 0;
	}
	return 1;
}


bool accept_literal_expression()
{
	if (accept(TK_ID))
	{

	}
	else if (accept(TK_STRING))
	{

	}
	else if (accept(TK_INT_LIT))
	{

	}
	else
	{
		return 0;
	}
	return 1;
}


bool accept_binary_expression()
{
	if (accept(TK_ASSIGN))
	{
		expression();
	}
	else if (accept(TK_PLUS))
	{
		expression();
	}
	else if (accept(TK_MINUS))
	{
		expression();
	}
	else if (accept(TK_STAR))
	{
		expression();
	}
	else if (accept(TK_DIV))
	{
		expression();
	}
	else if (accept(TK_ARROW))
	{
		expression();
	}
	else
	{
		return 0;
	}
	return 1;
}


/*
E: lit C
└: ( E ) C
└: U E C
*/
void expression()
{
	AST_CALL();
	if (accept_literal_expression())
	{
		expression1();
	}
	else if (accept(TK_OP))
	{
		expression();
		expect(TK_CP);
		expression1();
	}
	else if (accept_unary_expression())
	{
		expression();
		expression1();
	}
	else
	{
		printf("Error no expression found, %s\n", token.content);
	}
	AST_CALL_END();
}

/*
C: [ E ] C
└: ( A ) C
└: O E C
└:
*/
void expression1()
{
	AST_CALL();
	if (accept(TK_OSB))
	{
		expression();
		expect(TK_CSB);
		expression1();
	}
	else if (accept(TK_OP))
	{
		expression2();
		expect(TK_CP);
		expression1();
	}
	else if (accept_binary_expression())
	{
		expression();
		expression1();
	}
	else
	{
		printf("Error no expression1 found, %s\n", token.content);	
	}
	AST_CALL_END()
}

/*
B: lit C D
└: ( E ) C D
└: U E C D
*/
void expression2()
{
	AST_CALL();
	if (accept_literal_expression())
	{
		expression1();
		expression3();
	}
	else if (accept(TK_OP))
	{
		expression();
		expect(TK_CP);
		expression1();
		expression3();
	}
	else if (accept_unary_expression())
	{
		expression();
		expression1();
		expression3();
	}
	else
	{

	}
	AST_CALL_END()
}

/*
D: , B D
└:
*/
void expression3()
{
	AST_CALL();
	if (accept(TK_COMMA))
	{
		expression2();
		expression3();
	}
	else
	{

	}
	AST_CALL_END()
}