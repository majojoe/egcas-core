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

#ifndef EGCUTFCODEPOINT_H
#define EGCUTFCODEPOINT_H

#include <QRegularExpression>

class QString;

class EgcUtfCodepoint {
public:
        /**
         * @brief encodeToXml encodes all codepoints in the source string to xml/html encoded codepoints (e.g. '∰' to
         * '&#x2A0C;')
         * Attention: this class does currently not support non bmp characters (unicode caracters > 0xFFFF)
         * @param str the source string with unencoded utf-8 codepoints (e.g. 'test1 ∰ test2')
         * @return the xml/html encoded string (e.g. 'test1 &#x2A0C; test2')
         */
        static QString encodeToXml(const QString& str);
        /**
         * @brief decodeToUtf decodes all codepoints encoded in xml/html back to utf-8 (e.g. '&#x2A0C;' to '∰')
         * @param str the source string with xml/html encoded codepoints (e.g. 'test1 &#x2A0C; test2')
         * @return the result with utf-8 encoded codepoints (e.g. 'test1 ∰ test2')
         */
        static QString decodeToUtf(const QString& str);
private:
        static QRegularExpression s_replaceDec; ///< regex to replace e.g. &#10764; by '∰'
        static QRegularExpression s_replaceHex; ///< regex to replace e.g. &#x2A0C; by '∰'
        static bool s_regexInitalized;    ///< initialize regex?
};

#endif //#ifndef EGCUTFCODEPOINT_H
