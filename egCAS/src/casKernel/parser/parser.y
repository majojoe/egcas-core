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
%token <std::string> VARSUB "varsub";
%token <std::string> BUILTIN_FNCS "builtin_fncs";
%token PLUS "+";
%token MINUS "-";
%token MULTIPLICATION "*";
%token DIVISION "/";
%token EQUAL "=";
%token DEFINE ":";
%token COMMA ",";
%token LEFTPARENTESIS "(";
%token RIGHTPARENTHESIS ")";
%token LBRACKET_OP;
%token RBRACKET_OP;
%token EXPONENT "^";
%token SQROOT "_sqrt";
%token INTEGRAL "_integrate";
%token DIFFERENTIAL "_diff";
%token ROOT "_root";
%token EMPTY "_empty";
%token EMPTYBINOP "_emptybinop";
%token LOGARITHM "_log";
%token NATLOGARITHM "_ln";
%token RED_PARENTHESIS_R "_red_parenth_r";
%token RED_PARENTHESIS_L "_red_parenth_l";
%token ITERATOR_R;
%token ITERATOR_L;


%right "=" ":"
%nonassoc RED_PARENTHESIS_L RED_PARENTHESIS_R
%left "+" "-" "_emptybinop"
%left "*" "/"
%right "^"
%nonassoc "|" UMINUS
%nonassoc ITERATOR_R ITERATOR_L


%type<EgcNode*> expr;
%type<EgcNode*> equation;
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
  | formula equation END
      {
              #if (EGC_PARSER_DEBUG >= 1)
              cout << "***end" << endl;
              #endif //#if (EGC_PARSER_DEBUG >= 1)
              interpreter.setRootNode($2);
      }
;

equation: expr "=" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::EqualNode, $1, $3);}
        | expr ":" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::DefinitionNode, $1, $3);}
;

expr : expr "+" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::PlusNode, $1, $3);}
     | expr "-" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::MinusNode, $1, $3);}
     | expr "*" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::MultiplicationNode, $1, $3);}
     | expr "/" expr       {$$ = interpreter.addDivisionExpression($1, $3);}
     | LBRACKET_OP expr RBRACKET_OP "/" LBRACKET_OP expr RBRACKET_OP  {$$ = interpreter.addDivisionExpression($2, $6);}
     | expr "^" expr       {$$ = interpreter.addBinaryExpression(EgcNodeType::ExponentNode, $1, $3);}
     | expr EMPTYBINOP expr {$$ = interpreter.addBinaryExpression(EgcNodeType::BinEmptyNode, $1, $3);}
     | "(" expr ")"        {$$ = interpreter.addUnaryExpression(EgcNodeType::ParenthesisNode, $2);}
     | "-" expr %prec UMINUS {$$ = interpreter.addUnaryExpression(EgcNodeType::UnaryMinusNode, $2);}
     | NUMBER              {$$ = interpreter.addStringNode(EgcNodeType::NumberNode, $1);}
     | NAMES               {$$ = interpreter.addStringNode(EgcNodeType::VariableNode, $1);}
     | NAMES VARSUB        {$$ = interpreter.addStringNode(EgcNodeType::VariableNode, $1 + $2);}
     | NAMES "(" explist ")"{$$ = interpreter.addFunction($1, $3);}
     | EMPTY "(" explist ")"{$$ = interpreter.addFunction("", $3);}
     | BUILTIN_FNCS "(" expr ")"{$$ = interpreter.addBuiltinFunction($1, $3);}
     | LOGARITHM "(" expr ")" {$$ = interpreter.addUnaryExpression(EgcNodeType::LogNode, $3);}
     | NATLOGARITHM "(" expr ")" {$$ = interpreter.addUnaryExpression(EgcNodeType::NatLogNode, $3);}
     | "_sqrt" "(" expr ")" {$$ = interpreter.addSqrtExpression($3);}
     | INTEGRAL "(" explist ")" {$$ = interpreter.changeFlexExpressionType(EgcNodeType::IntegralNode, $3);}
     | DIFFERENTIAL "(" explist ")" {$$ = interpreter.addDifferentialExpression($3);}
     | "_root" "(" expr "," expr ")" {$$ = interpreter.addBinaryExpression(EgcNodeType::RootNode, $3, $5);}
     | "_root" "(" LBRACKET_OP expr RBRACKET_OP "," expr ")" {$$ = interpreter.addBinaryExpression(EgcNodeType::RootNode, $4, $7);}
     | "_empty"            {$$ = interpreter.addEmptyNode();}
     | expr ITERATOR_L      {$$ = interpreter.updateIterator($1, 2);}
     | ITERATOR_R expr      {$$ = interpreter.updateIterator($2, 1);}
     | "_red_parenth_l" expr {$$ = interpreter.addUnaryExpression(EgcNodeType::LParenthesisNode, $2);}
     | expr "_red_parenth_r" {$$ = interpreter.addUnaryExpression(EgcNodeType::RParenthesisNode, $1);}
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
