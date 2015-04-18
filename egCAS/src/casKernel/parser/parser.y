/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Krzysztof Narkiewicz <krzysztof.narkiewicz@ezaquarii.com>
 * Copyright (c) 2015 Johannes Maier <maier_jo@gmx.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines
%define parser_class_name { MaximaParser }
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { CASParser }
%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdint.h>
    #include "command.h"
    #include "../../structural/egcnodes.h"

    using namespace std;

    namespace CASParser {
        class MaximaScanner;
        class Interpreter;
    }
}

// Bison calls yylex() function that must be provided by us to suck tokens
// from the scanner. This block will be placed at the beginning of IMPLEMENTATION file (cpp).
// We define this function here (function! not method).
// This function is called only inside Bison, so we make it static to limit symbol visibility for the linker
// to avoid potential linking conflicts.
%code top
{
    #include <iostream>
    #include "scanner.h"
    #include "parser.hpp"
    #include "interpreter.h"
    #include "location.hh"
    
    // yylex() arguments are defined in parser.y
    static CASParser::MaximaParser::symbol_type yylex(CASParser::MaximaScanner &scanner, CASParser::Interpreter &interpreter) {
        return scanner.get_next_token();
    }
    
    // you can accomplish the same thing by inlining the code using preprocessor
    // x and y are same as in above static function
    // #define yylex(x, y) scanner.get_next_token()
    
    using namespace CASParser;
}

%lex-param { CASParser::MaximaScanner &scanner }
%lex-param { CASParser::Interpreter &interpreter }
%parse-param { CASParser::MaximaScanner &scanner }
%parse-param { CASParser::Interpreter &interpreter }
%locations
%define parse.trace
%define parse.error verbose

%define api.token.prefix {TOKEN_}

%token END 0 "end of file"
%token <std::string> NUMBER "number";
%token <char> OPERATOR "operator";
%token <std::string> NAMES "names";
%token PLUS "+";
%token MINUS "-";
%token MULTIPLICATION "*";
%token DIVISION "/";
%token EQUAL "=";
%token COMMA ",";
%token LEFTPARENTESIS "(";
%token RIGHTPARENTHESIS ")";

%left "+" "-"
%left "*" "/"


%type<EgcExpressionNode*> expr;

/*%type< CASParser::Command > command;
%type< std::vector<uint64_t> > arguments;*/

%start formula

%%

 /*use always the interpreter methods to buildup an AST*/

formula : /*nothing*/
        {
                #if (EGC_PARSER_DEBUG >= 1)
                cout << "***start" << endl; /*interpreter.clear();*/
                #endif //#if (EGC_PARSER_DEBUG >= 1)
        }
  | formula expr END
        {
                #if (EGC_PARSER_DEBUG >= 1)
                cout << "***end" << endl; /*interpreter.str();*/
                #endif //#if (EGC_PARSER_DEBUG >= 1)
        }
  ;


 /*#warning change this if plus, minus, etc. node exist*/
expr : expr "+" expr    {cout << "+" << endl; $$ = interpreter.addBinaryExpression(EgcExpressionNodeType::RootNode, $1, $3);}
  | expr "-" expr       {cout << "-" << endl; $$ = interpreter.addBinaryExpression(EgcExpressionNodeType::RootNode, $1, $3);}
  | expr "*" expr       {cout << "*" << endl; $$ = interpreter.addBinaryExpression(EgcExpressionNodeType::RootNode, $1, $3);}
  | expr "/" expr       {cout << "/" << endl; $$ = interpreter.addBinaryExpression(EgcExpressionNodeType::RootNode, $1, $3);}
  | "(" expr ")"        {cout << "( " << $2 << " )" << endl; $$ = interpreter.addUnaryExpression(EgcExpressionNodeType::ParenthesisNode, $2);}
  | NUMBER              {cout << $1 << endl;}
  ;

/*program :   {
                cout << "*** RUN ***" << endl;
                cout << "Type function with list of parmeters. Parameter list can be empty" << endl
                     << "or contain positive integers only. Examples: " << endl
                     << " * function()" << endl
                     << " * function(1,2,3)" << endl
                     << "Terminate listing with ; to see parsed AST" << endl
                     << "Terminate parser with Ctrl-D" << endl;
                
                cout << endl << "prompt> ";
                
                interpreter.clear();
            }
        | program command
            {
                const Command &cmd = $2;
                cout << "command parsed, updating AST" << endl;
                interpreter.addCommand(cmd);
                cout << endl << "prompt> ";
            }
        | program SEMICOLON
            {
                cout << "*** STOP RUN ***" << endl;
                cout << interpreter.str() << endl;
            }
        ;


 command : STRING LEFTPAR RIGHTPAR
        {
            string &id = $1;
            cout << "ID: " << id << endl;
            $$ = Command(id);
        }
    | STRING LEFTPAR arguments RIGHTPAR
        {
            string &id = $1;
            const std::vector<uint64_t> &args = $3;
            cout << "function: " << id << ", " << args.size() << endl;
            $$ = Command(id, args);
        }
    ;

 arguments : NUMBER
        {
            uint64_t number = $1;
            $$ = std::vector<uint64_t>();
            $$.push_back(number);
            cout << "first argument: " << number << endl;
        }
    | arguments COMMA NUMBER
        {
            uint64_t number = $3;
            std::vector<uint64_t> &args = $1;
            args.push_back(number);
            $$ = args;
            cout << "next argument: " << number << ", arg list size = " << args.size() << endl;
        }
    ;*/
    
%%

// Bison expects us to provide implementation - otherwise linker complains
void CASParser::MaximaParser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from interpreter class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
        cout << "Error: " << message << endl << "Error location: " << interpreter.location() << endl;
}
