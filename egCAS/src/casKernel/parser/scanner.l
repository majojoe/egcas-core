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
        #define YY_USER_ACTION m_driver.increaseLocation(yyleng); scanner_dbg(yytext);
	
	// !!!WARNING!!!
	// Location API is used, but the location is not initialized, 'cause I'm lazy. When making
	// a token with make_{something} method you can pass detailed token location. Current location
	// is accessible with m_driver.location() method. All puzzle elements are there - just
	// pass location value in every action code block below. I'm going to waste more time writing
	// this excuse than putting this boilerplate below...
	//
	// Location class can be found in location.hh and posistion.hh files. It's just a bit too much
	// boilerplate for this small example. Bummer.

        void scanner_dbg(char* tokenText)
        {
                #if (EGC_SCANNER_DEBUG >= 3)
                        cout << "Lexer: matched token -> [" << tokenText << "]" << endl;
                #endif //#if (EGC_SCANNER_DEBUG >= 3)
        }

%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="MaximaScanner"
%option prefix="CASParser_"


EXPONENT     ([Eeb][-+]?[0-9]+)

%%

 /*operators*/
"+"     { return CASParser::MaximaParser::make_PLUS(CASParser::location()); }
"-"     { return CASParser::MaximaParser::make_MINUS(CASParser::location()); }
"*"     { return CASParser::MaximaParser::make_MULTIPLICATION(CASParser::location()); }
"/"     { return CASParser::MaximaParser::make_DIVISION(CASParser::location()); }
"="     { return CASParser::MaximaParser::make_EQUAL(CASParser::location()); }
":"     { return CASParser::MaximaParser::make_DEFINE(CASParser::location()); }
","     { return CASParser::MaximaParser::make_COMMA(CASParser::location()); }
"("     { return CASParser::MaximaParser::make_LEFTPARENTESIS(CASParser::location()); }
")"     { return CASParser::MaximaParser::make_RIGHTPARENTHESIS(CASParser::location()); }
"^" |
"**"    { return CASParser::MaximaParser::make_EXPONENT(CASParser::location()); }

 /*numbers*/
[0-9]+"."[0-9]*{EXPONENT}? |
"."?[0-9]+{EXPONENT}? { return CASParser::MaximaParser::make_NUMBER(yytext, CASParser::location()); }

 /*builtin functions*/
sin |
cos {
        return CASParser::MaximaParser::make_BUILTIN_FNCS(yytext, CASParser::location());
 }

 /*names*/
[a-zA-Z_][a-zA-Z0-9_]* { return CASParser::MaximaParser::make_NAMES(yytext, CASParser::location());}

[\n\t ] { /*ignore whitechar*/ }

 /*all other not recognized characters*/
. {
        location loc;
        throw CASParser::MaximaParser::syntax_error(loc, "Lexer: unrecognized character: " + std::string(yytext, yyleng));
 }
            
<<EOF>> { return yyterminate(); }


%%
