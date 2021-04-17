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
	newt(token, str);


#define lexer_advance() c = content[++stri]
#define lexer_read_ahead() content[stri + 1]
#define newt(type, string) (tokens[i++] = (Token){type, string}) //new token


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
			case ';': newt(TK_SCLN, ";"); break;
			case '(': newt(TK_OP, "("); break;
			case ')': newt(TK_CP, ")"); break;
			case '{': newt(TK_OB, "{"); break;
			case '}': newt(TK_CB, "}"); break;
			case '[': newt(TK_OSB, "["); break;
			case ']': newt(TK_CSB, "]"); break;
			case ',': newt(TK_COMMA, ","); break;
			case '+': newt(TK_PLUS, "+"); break;
			case '*': newt(TK_STAR, "*"); break;
			case '/': newt(TK_DIV, "/"); break;
			case '!': newt(TK_NOT, "!"); break;
			case '&': newt(TK_AMPERCENT, "&"); break;
			case '@': newt(TK_AT, "@"); break;
			case '.': newt(TK_POINT, "."); break;
			case '<': newt(TK_LESSTHAN, "<"); break;
			case '>': newt(TK_MORETHAN, ">"); break;
			case '=': 
				if (lexer_read_ahead() == '=')
				{
					newt(TK_EQUALS, "=="); 
					lexer_advance();
				}
				else
				{
					newt(TK_ASSIGN, "="); 
				}
				break;
			case '-': 
				if (lexer_read_ahead() == '>')
				{
					newt(TK_ARROW, "->"); 
					lexer_advance();
				}
				else
				{
					newt(TK_MINUS, "-"); 
				}
				break;
			default:
				printf("Unknown symbol parsed: %c\n", c);
				assert(0);
		}
		lexer_advance(); 
	}
	newt(TK_EOF, "EOF");

	for (U32 tk_i=0; tk_i < i; ++tk_i)
	{
		if (tokens[tk_i].type == TK_ID)
		{
			if (isupper(tokens[tk_i].content[0]))
			{
				tokens[tk_i].type = TK_TYPE;
				continue;
			}
			char *keywords[] = {"if", "while", "for", "ret", "struct"};
			TokenType matches[] = {TK_IF, TK_WHILE, TK_FOR, TK_RET, TK_STRUCT};
			for (unsigned int i=0; i < sizeof(matches) / sizeof(matches[0]); ++i)
			{
				if (!strcmp(keywords[i], tokens[tk_i].content))
				{
					tokens[tk_i].type = matches[i];
				}
			}
		}
	}

	return tokens;
}