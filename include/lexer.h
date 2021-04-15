#ifndef LEXER_H
#define LEXER_H
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "types.h"

#define MAX_TOKENS 65535
#define MAX_ID_SIZE 32
#define MAX_INT_LITERAL_SIZE 16
#define MAX_STRING_SIZE 64


typedef enum
{
//other
	TK_NULL,
	TK_EOF,
	
	TK_ID,
	TK_STRING,
	TK_INT_LIT,

//terminals
	TK_OP,
	TK_CP,
	TK_OB,
	TK_CB,
	TK_OSB,
	TK_CSB,
	TK_SCLN,


//binary expressions
	TK_ASSIGN,
	TK_PLUS,
	TK_MINUS,
	TK_STAR,
	TK_COMMA,
	TK_DIV,
	TK_ARROW,
	TK_POINT,
	TK_EQUALS,
	TK_LESSTHAN,
	TK_MORETHAN,

//unary expressions
	TK_NOT,
	TK_AMPERCENT,
	TK_AT,


//control statements
	TK_IF,
	TK_WHILE,
	TK_FOR,
	TK_RET,

// 
	TK_STRUCT,

//construct type indicators
	TK_TYPE
	
} TokenType;


typedef struct
{
	TokenType type;
	char const *content;
}
Token;


Token* lexer_collect_tokens(char const *content);


#endif