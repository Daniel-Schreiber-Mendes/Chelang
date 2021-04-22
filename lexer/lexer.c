#include "lexer.h"

//if (true){}\ //used so that this macro can be used in switch case labels
#define collect(condition, token)\
	if (true){}\
	unsigned int capacity = 4, size = 0;\
	char *str = malloc(capacity * sizeof(char));\
	while (condition)\
	{\
		if (size + 1 > capacity)\
			str = realloc(str, capacity = capacity * 2);\
		str[size++] = c;\
		lexer_advance();\
	}\
	str[size] = '\0';\
	newt(token, str);


#define lexer_advance()\
	({\
		assert(stri < strsize);\
		c = content[++stri];\
	})
#define lexer_read_ahead() content[stri + 1]
#define newt(type, string) (vector_push_back(&tokens, Token, ((Token){type, string}))) //new token


Vector lexer_collect_tokens(char const *content)
{
	U16 stri = 0; //index into string
	unsigned int strsize = strlen(content);
	Vector tokens;
	vector_construct(&tokens, sizeof(Token));

	char c = content[0];
	while (c != '\0')
	{
		if (isalpha(c) || c == '_') 
		{
			collect((isalpha(c) || c == '_' || c == '@'), TK_ID);
		}
		else if (isdigit(c))
		{
			collect((isdigit(c)), TK_INT_LIT);
		}
		else
		{
			switch (c)
			{
				case ' ':
				case '\n':
				case '\t': break;
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
				case '"': collect((c == '"'), TK_STRING); break;
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
	}
	newt(TK_EOF, "EOF");

	vector_pforeach(&tokens, Token*, t)
	{
		if (t->type != TK_ID)
			continue;
		
		if (isupper(t->content[0]))
		{
			t->type = TK_TYPE;
			continue;
		}

		char *keywords[] = {"if", "else", "while", "for", "ret", "struct"};
		TokenType matches[] = {TK_IF, TK_ELSE, TK_WHILE, TK_FOR, TK_RET, TK_STRUCT};
		for (unsigned int i=0; i < sizeof(matches) / sizeof(matches[0]); ++i)
		{
			if (!strcmp(keywords[i], t->content))
			{
				t->type = matches[i];
			}
		}	
	}

	return tokens;
}