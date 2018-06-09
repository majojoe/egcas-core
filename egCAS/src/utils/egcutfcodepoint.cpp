/*
 * Copyright (c) 2016, Johannes Maier <maier_jo@gmx.de>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 * 
 * Neither the name of the egCAS nor the names of its
 *  contributors may be used to endorse or promote products derived from
 *  this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/


#include <QString>
#include <QStringBuilder>
#include <QRegularExpression>
#include "egcutfcodepoint.h"


QRegularExpression EgcUtfCodepoint::s_replaceDec = QRegularExpression("&#([0-9]+);");
QRegularExpression EgcUtfCodepoint::s_replaceHex = QRegularExpression("&#x([0-9a-fA-F]+);");
bool EgcUtfCodepoint::s_regexInitalized = false;


QString EgcUtfCodepoint::encodeToXml(const QString &str)
{
        QString retval(str); //it's faster to preload the string in most cases
        QString tmp;
        int len =  str.length();
        int i = 0;

        while( i < len ) {
                if(  retval[i].unicode() > 0xFF) {
                        tmp = "&#" % QString::number( retval[i].unicode()) % ";";
                        retval.replace(i, 1, tmp);
                        int ln = tmp.length();
                        i += ln;
                        len += ln - 1;
                } else {
                        i++;
                }
        }

        return  retval;
}

QString EgcUtfCodepoint::decodeToUtf(const QString &str)
{
        QString  retval(str);
        QRegularExpressionMatch match;

        if (!s_regexInitalized) { // compile regex
                s_replaceDec.optimize();
                s_replaceHex.optimize();
                s_regexInitalized = true;
        }

        //for dec values
        while( (match =  s_replaceDec.match(retval, 0)).hasMatch() ) {
                retval =  retval.replace( match.capturedStart(0), match.capturedLength(0),
                                          QChar( match.captured(1).toInt(0,10)));
        }

        //for hex values
        while( (match =  s_replaceHex.match(retval, 0)).hasMatch() ) {
                retval =  retval.replace( match.capturedStart(0), match.capturedLength(0),
                                          QChar( match.captured(1).toInt(0,16)));
        }

        return  retval;
}



