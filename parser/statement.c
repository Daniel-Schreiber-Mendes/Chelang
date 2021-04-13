#include "parser.h"


void statement(Ast *ast)
{
    if (accept(TK_OB)) //compound statement
    {	
        if (accept(TK_CB)) return; //if statement is empty

        ast = ast_add_child(ast, C_SCOPE, "C_SCOPE");
        while (!check(TK_CB))
        {
            statement(ast);
        }
        expect(TK_CB);
    } 
    else if(accept_var_fnc_type_def(ast))
    {

    }
    else if (accept(TK_IF))
    {
        expect(TK_OP);
        ast = ast_add_child(ast, C_IF, "C_IF");
        expression(ast);
        expect(TK_CP);
        statement(ast);
    }
    else if (accept(TK_WHILE))
    {
        expect(TK_OP);
        ast = ast_add_child(ast, C_WHILE, "C_WHILE");
        expression(ast);
        expect(TK_CP);
        statement(ast);
    }
    else if (accept(TK_FOR))
    {
        //not yet implemented
    }
    else if (accept(TK_RET))
    {
        ast = ast_add_child(ast, C_RET, "C_RET");
        if (!accept(TK_SCLN))
        {
            expression(ast);
        }
        expect(TK_SCLN);
    }
    else if(accept(TK_SCLN)) // nop
    {

    }
    else //expression statement
    {
        expression(ast);
        expect(TK_SCLN);
    }
}


bool accept_var_fnc_type_def(Ast *ast)
{
    if (accept(TK_STRUCT))
    {
        ast = ast_add_child(ast, C_STRUCT_DEF, "C_STRUCT_DEF");
        expect(TK_TYPE);
        ast_add_token(ast, get_token(-1));
        expect(TK_OB);
        while (accept(TK_TYPE))
        {
            expect(TK_ID);
            ast_add_token(ast, get_token(-2));
            ast_add_token(ast, get_token(-1));
            if (!check(TK_CB))
            {
                expect(TK_COMMA);
            }
        }
        expect(TK_CB);
    }
    else if(accept(TK_TYPE))
    {
        expect(TK_ID);
        if (accept(TK_ASSIGN)) //var definition with initialization
        {
            ast = ast_add_child(ast, C_VAR_DEF_INIT, "C_VAR_DEF_INIT");
            ast_add_token(ast, get_token(-3));
            ast_add_token(ast, get_token(-2));
            expression(ast);
            expect(TK_SCLN);
        }
        else if (accept(TK_OP)) //function definition
        {
            ast = ast_add_child(ast, C_FUNC_DEF, "C_FUNC_DEF");
            ast_add_token(ast, get_token(-3));
            ast_add_token(ast, get_token(-2));
            while (accept(TK_TYPE))
            {
                expect(TK_ID);
                ast_add_token(ast, get_token(-2));
                ast_add_token(ast, get_token(-1));
                if (!check(TK_CP))
                {
                    expect(TK_COMMA);
                }
            }
            expect(TK_CP);
            statement(ast);
        }
        else 
        {
            ast = ast_add_child(ast, C_VAR_DEF, "C_VAR_DEF");
            ast_add_token(ast, get_token(-2));
            ast_add_token(ast, get_token(-1));                
            expect(TK_SCLN); //var definition
        }
        return 1;
    }
    return 0;
}

void var_fnc_type_def(Ast *ast)
{
    if (accept(TK_STRUCT))
    {
        ast = ast_add_child(ast, C_STRUCT_DEF, "C_STRUCT_DEF");
        expect(TK_TYPE);
        ast_add_token(ast, get_token(-1));
        expect(TK_OB);
        while (accept(TK_TYPE))
        {
            expect(TK_ID);
            ast_add_token(ast, get_token(-2));
            ast_add_token(ast, get_token(-1));
            if (!check(TK_CB))
            {
                expect(TK_COMMA);
            }
        }
        expect(TK_CB);
    }
    else
    {
        expect(TK_TYPE);
        expect(TK_ID);
        if (accept(TK_ASSIGN)) //var definition with initialization
        {
            ast = ast_add_child(ast, C_VAR_DEF_INIT, "C_VAR_DEF_INIT");
            ast_add_token(ast, get_token(-3));
            ast_add_token(ast, get_token(-2));
            expression(ast);
            expect(TK_SCLN);
        }
        else if (accept(TK_OP)) //function definition
        {
            ast = ast_add_child(ast, C_FUNC_DEF, "C_FUNC_DEF");
            ast_add_token(ast, get_token(-3));
            ast_add_token(ast, get_token(-2));
            while (accept(TK_TYPE))
            {
                expect(TK_ID);
                ast_add_token(ast, get_token(-2));
                ast_add_token(ast, get_token(-1));
                if (!check(TK_CP))
                {
                    expect(TK_COMMA);
                }
            }
            expect(TK_CP);
            statement(ast);
        }
        else 
        {
            ast = ast_add_child(ast, C_VAR_DEF, "C_VAR_DEF");
            ast_add_token(ast, get_token(-2));
            ast_add_token(ast, get_token(-1));
            expect(TK_SCLN); //var definition
        }
    }
}