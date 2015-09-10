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
    #include "egcnodes.h"

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
    #include "parser/scanner.h"
    #include "parser.hpp"
    #include "parser/interpreter.h"
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
%token <std::string> BUILTIN_FNCS "builtin_fncs";
%token PLUS "+";
%token MINUS "-";
%token MULTIPLICATION "*";
%token DIVISION "/";
%token EQUAL "=";
%token DEFINE ":"
%token COMMA ",";
%token LEFTPARENTESIS "(";
%token RIGHTPARENTHESIS ")";
%token EXPONENT "^"
%token SQROOT "sqrt"
%token INTEGRAL "_integrate"
%token DIFFERENTIAL "_diff"
%token ROOT "_root" //only for debugging purposes
%token EMPTY "_empty" //only for debugging purposes



%right "=" ":"
%left "+" "-"
%left "*" "/"
%left "^"
%nonassoc "|" UMINUS


%type<EgcNode*> expr;
%type<EgcArgumentsNode*> explist;

%start formula

%%

 /*use always the interpreter methods to buildup an AST*/

formula : /*nothing*/
        {
                #if (EGC_PARSER_DEBUG >= 1)
                cout << "***start" << endl;
                #endif //#if (EGC_PARSER_DEBUG >= 1)                
                interpreter.clear();
        }
  | formula expr END
        {
                #if (EGC_PARSER_DEBUG >= 1)
                cout << "***end" << endl;
                #endif //#if (EGC_PARSER_DEBUG >= 1)
                interpreter.setRootNode($2);
        }
;

expr : expr "+" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::PlusNode, $1, $3);}
     | expr "-" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::MinusNode, $1, $3);}
     | expr "*" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::MultiplicationNode, $1, $3);}
     | expr "/" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::DivisionNode, $1, $3);}
     | expr "^" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::ExponentNode, $1, $3);}
     | "(" expr ")"        {$$ = interpreter.addUnaryExpression(EgcNodeType::ParenthesisNode, $2);}
     | "-" expr %prec UMINUS {$$ = interpreter.addUnaryExpression(EgcNodeType::UnaryMinusNode, $2);}
     | expr "=" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::EqualNode, $1, $3);}
     | expr ":" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::DefinitionNode, $1, $3);}
     | NUMBER              {$$ = interpreter.addStringNode(EgcNodeType::NumberNode, $1);}
     | NAMES               {$$ = interpreter.addStringNode(EgcNodeType::VariableNode, $1);}
     | NAMES "(" explist ")"{$$ = interpreter.addFunction($1, $3);}
     | BUILTIN_FNCS "(" explist ")"{$$ = interpreter.addBuiltinFunction($1, $3);}
     | "sqrt" "(" expr ")" {$$ = interpreter.addSqrtExpression($3);}
     | "_root" "(" expr "," expr ")" {$$ = interpreter.addBinaryExpression(EgcNodeType::RootNode, $3, $5);}  //only for debugging purposes
     | "_empty"            {$$ = interpreter.addEmptyNode();}   //only for debug purposes
     | INTEGRAL "(" explist ")" {$$ = interpreter.changeFlexExpressionType(EgcNodeType::IntegralNode, $3);}
     | DIFFERENTIAL "(" explist ")" {$$ = interpreter.addDifferentialExpression($3);}
;
    
explist: expr            {$$ = interpreter.createArgList($1);}
     | expr "," explist    {$$ = interpreter.addArgument($1, $3);}
;

%%

// Bison expects us to provide implementation - otherwise linker complains
void CASParser::MaximaParser::error(const location &loc , const std::string &message) {
        //remove dangling nodes that already have been allocated during AST built up
        interpreter.deleteDanglingNodes();
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from interpreter class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
        cout << "Error: " << message << endl << "Error location: " << interpreter.location() << endl;
}
