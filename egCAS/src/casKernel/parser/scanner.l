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

%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "interpreter.h"
	#include "parser.hpp"
	#include "location.hh"

	using namespace std;

	// Original yyterminate() macro returns int. Since we're using Bison 3 variants
        // as tokens, we must redefine it to change type from `int` to `MaximaParser::semantic_type`
        #define yyterminate() CASParser::MaximaParser::make_END(CASParser::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver.increaseLocation(yyleng);
	
	// !!!WARNING!!!
	// Location API is used, but the location is not initialized, 'cause I'm lazy. When making
	// a token with make_{something} method you can pass detailed token location. Current location
	// is accessible with m_driver.location() method. All puzzle elements are there - just
	// pass location value in every action code block below. I'm going to waste more time writing
	// this excuse than putting this boilerplate below...
	//
	// Location class can be found in location.hh and posistion.hh files. It's just a bit too much
	// boilerplate for this small example. Bummer.

        #define PARSER_DEBUG 1
        #ifdef PARSER_DEBUG
        #define parser_dbg(x, y) do{                                                                     \
                                              cout << "Scanner: "  << y << "[" << x << "]" << endl;     \
                              }while(0)
        #else //#ifdef PARSER_DEBUG
        #define parser_dbg(x, y) do{                                                                     \
                              }while(0)
        #endif //#ifdef PARSER_DEBUG
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="MaximaScanner"
%option prefix="CASParser_"


EXPONENT     ([Eeb][-+]?[0-9]+)

%%

"," |
"=" |
"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")"     {
                parser_dbg(yytext, "operator");
                //return CASParser::MaximaParser::make_OPERATOR(yytext[0], CASParser::location());
                return CASParser::MaximaParser::basic_symbol (TOKEN_OPERATOR, yytext[0], CASParser::location());
        }

[0-9]+"."[0-9]*{EXPONENT}? |
"."?[0-9]+{EXPONENT}? {
                parser_dbg(yytext, "number");
                return CASParser::MaximaParser::make_NUMBER(yytext, CASParser::location());
        }

 /*variables*/
[a-zA-Z][a-zA-Z0-9]* {
                parser_dbg(yytext, "variable");
                return CASParser::MaximaParser::make_VARIABLE(yytext, CASParser::location());
        }
                                    
[\n\t ] { /*ignore whitechar*/ }

.       {
                location loc;
                throw CASParser::MaximaParser::syntax_error(loc, "Lexer: unrecognized character: " + std::string(yytext, yyleng));
        }
            
<<EOF>> { return yyterminate(); }


%%
