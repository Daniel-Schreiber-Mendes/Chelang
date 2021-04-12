#include "lexer.h"

#define collect(max_size, condition, token)\
	char *str = malloc(max_size * sizeof(char));\
	U8 size = 0;\
	while (condition)\
	{\
		str[size++] = c;\
		lexer_advance();\
	}\
	str[size] = '\0';\
	tokens[i++] = (Token){token, str};


#define lexer_advance() c = content[++stri]
#define lexer_read_ahead() content[stri + 1]


Token* lexer_collect_tokens(char const *content)
{
	U16 stri = 0; //index into string
	U16 i = 0; //index into token array
	Token *tokens = malloc(sizeof(Token) * MAX_TOKENS);


	char c = content[0];
	while (c != '\0')
	{
		if (c == ' ' || c == '\n' || c == '\t') 
		{
			lexer_advance();
			continue;
		}
		if (isalpha(c) || c == '_')
		{
			collect(MAX_ID_SIZE, (isalpha(c) || c == '_'), TK_ID);
			while (c == '@')
			{
				str[size++] = '@';
				str[size] = '\0';
				lexer_advance();
			}
			continue;
		}
		if (isdigit(c)) //if has 1 digit faisl
		{
			collect(MAX_INT_LITERAL_SIZE, (isdigit(c)), TK_INT_LIT);
			continue;
		}
		if (c == '"')
		{
			collect(MAX_STRING_SIZE, (c == '"'), TK_STRING);
			continue;
		}

		switch (c)
		{
			case ' ':  break;
			case '\n': break;
			case ';': tokens[i++] = (Token){TK_SCLN, ";"}; break;
			case '(': tokens[i++] = (Token){TK_OP, "("}; break;
			case ')': tokens[i++] = (Token){TK_CP, ")"}; break;
			case '{': tokens[i++] = (Token){TK_OB, "{"}; break;
			case '}': tokens[i++] = (Token){TK_CB, "}"}; break;
			case '[': tokens[i++] = (Token){TK_OSB, "["}; break;
			case ']': tokens[i++] = (Token){TK_CSB, "]"}; break;
			case ',': tokens[i++] = (Token){TK_COMMA, ","}; break;
			case '+': tokens[i++] = (Token){TK_PLUS, "+"}; break;
			case '*': tokens[i++] = (Token){TK_STAR, "*"}; break;
			case '/': tokens[i++] = (Token){TK_DIV, "/"}; break;
			case '!': tokens[i++] = (Token){TK_NOT, "!"}; break;
			case '&': tokens[i++] = (Token){TK_AMPERCENT, "&"}; break;
			case '@': tokens[i++] = (Token){TK_AT, "@"}; break;
			case '.': tokens[i++] = (Token){TK_POINT, "."}; break;
			case '<': tokens[i++] = (Token){TK_LESSTHAN, "<"}; break;
			case '>': tokens[i++] = (Token){TK_MORETHAN, ">"}; break;
			case '=': 
				if (lexer_read_ahead() == '=')
				{
					tokens[i++] = (Token){TK_EQUALS, "=="}; 
					lexer_advance();
				}
				else
				{
					tokens[i++] = (Token){TK_ASSIGN, "="}; 
				}
				break;
			case '-': 
				if (lexer_read_ahead() == '>')
				{
					tokens[i++] = (Token){TK_ARROW, "->"}; 
					lexer_advance();
				}
				else
				{
					tokens[i++] = (Token){TK_MINUS, "-"}; 
				}
				break;
			default:
				printf("Unknown symbol parsed: %c\n", c);
				assert(0);
		}
		lexer_advance(); 
	}
	tokens[i++] = (Token){TK_EOF, "EOF"};

	for (U32 tk_i=0; tk_i < i; ++tk_i)
	{
		if (tokens[tk_i].type == TK_ID)
		{
			if (isupper(tokens[tk_i].content[0]))
			{
				tokens[tk_i].type = TK_TYPE;
			}
			else if (!strcmp(tokens[tk_i].content, "if"))
			{
				tokens[tk_i].type = TK_IF;
			}
			else if (!strcmp(tokens[tk_i].content, "while"))
			{
				tokens[tk_i].type = TK_WHILE;
			}
			else if (!strcmp(tokens[tk_i].content, "for"))
			{
				tokens[tk_i].type = TK_FOR;
			}
			else if (!strcmp(tokens[tk_i].content, "ret"))
			{
				tokens[tk_i].type = TK_RET;
			}
		}
	}

	return tokens;
}