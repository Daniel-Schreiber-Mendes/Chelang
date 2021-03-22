#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include "lexer.h"
#include <stdio.h>
#include <string.h>


#define lexer_advance(l) c = content[++stri]


TokenArray lexer_collect_tokens(char const *content)
{
	unsigned int stri = 0, i = 0; //index into string, index into token array
	Token *tokens = malloc(sizeof(Token) * MAX_TOKENS);


	char c = content[0];
	while (c != '\0')
	{
		if (c == ' ' || c == '\n' || c == '\t') 
		{
			lexer_advance(l);
			continue;
		}

		if (isalpha(c))
		{
			char *id = malloc(MAX_ID_SIZE * sizeof(char));
			unsigned char size = 0;
			while (isalpha(c))
			{
				id[size++] = c;
				lexer_advance(l);
			}
			id[size] = '\0';
			tokens[i++] = (Token){TK_ID, id, 0};
			continue;
		}

		if (isdigit(c)) //if has 1 digit faisl
		{
			char *int_lit = malloc(MAX_INT_LITERAL_SIZE * sizeof(char));
			unsigned char size = 0;
			while (isdigit(c))
			{
				int_lit[size++] = c;
				lexer_advance(l);
			}
			int_lit[size] = '\0';
			if (size == 1)
			{
				tokens[i++] = (Token){TK_INT_LIT, int_lit, atoi(int_lit)};
			}
			else
			{
				tokens[i++] = (Token){TK_INT_LIT, int_lit, strtol(int_lit, &int_lit + size, 10)};
			}
			continue;
		}


		if (c == '"')
		{
			char *string = malloc(MAX_STRING_SIZE * sizeof(char));
			unsigned char string_size = 0;
			while (c != '"')
			{
				string[string_size++] = c;
				lexer_advance(l);
			}
			string[string_size] = '\0';
			tokens[i++] = (Token){TK_STRING, string, 0};
			continue;
		}

		switch (c)
		{
			case ' ':  break;
			case '\n': break;
			case '=': tokens[i++] = (Token){TK_ASSIGN, "=", 0}; break;
			case ';': tokens[i++] = (Token){TK_SCLN, ";", 0}; break;
			case '(': tokens[i++] = (Token){TK_OP, "(", 0}; break;
			case ')': tokens[i++] = (Token){TK_CP, ")", 0}; break;
			case '{': tokens[i++] = (Token){TK_OB, "{", 0}; break;
			case '}': tokens[i++] = (Token){TK_CB, "}", 0}; break;
			case '[': tokens[i++] = (Token){TK_OSB, "[", 0}; break;
			case ']': tokens[i++] = (Token){TK_CSB, "]", 0}; break;
			case ',': tokens[i++] = (Token){TK_COMMA, ",", 0}; break;
			case '+': tokens[i++] = (Token){TK_PLUS, "+", 0}; break;
			case '-': tokens[i++] = (Token){TK_MINUS, "-", 0}; break;
			case '*': tokens[i++] = (Token){TK_STAR, "*", 0}; break;
			case '/': tokens[i++] = (Token){TK_DIV, "/", 0}; break;
			case '!': tokens[i++] = (Token){TK_NOT, "!", 0}; break;
			case '&': tokens[i++] = (Token){TK_AMPERCENT, "&", 0}; break;
			case '@': tokens[i++] = (Token){TK_AT, "@", 0}; break;
			default:
				printf("Unknown symbol parsed: %c\n", c);
				assert(0);
		}
		lexer_advance(l); 
	}
	tokens[i++] = (Token){TK_EOF, "EOF", 0};

	for (unsigned int tk_i=0; tk_i < i; ++tk_i)
	{
		if (tokens[tk_i].type == TK_ID)
		{
			if (isupper(tokens[tk_i].content[0]))
			{
				tokens[tk_i].type = TK_TYPE;
			}
			else if (!strcmp(tokens[tk_i].content, "fnc"))
			{
				tokens[tk_i].type = TK_FNC;
			}
		}
	}

	return (TokenArray){tokens, i};
}


//anonymus functions
//functions inside other functions
//no header
//export module
//@ instead of *