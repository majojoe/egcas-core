/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#ifndef EGCKERNELPARSER_H
#define EGCKERNELPARSER_H

class EgcNode;

#include <QString>
#include <QScopedPointer>
#include "interpreter.h"

/**
 * @brief The EgcKernelParser class encapsulates the parsing of the cas kernel output and creates a node tree thereof.
 */
class EgcKernelParser
{
public:
        EgcKernelParser();
        virtual ~EgcKernelParser() {}

        /**
         * @brief parseKernelOutput parse the cas kernel output and generate a expression tree from it.
         * @param strToParse the output of the cas kernel to generate a tree from
         * @return the result (tree) of the parsing of the cas kernel output
         */
        EgcNode* parseKernelOutput(const QString& strToParse);

        /**
         * @brief getErrorMessage the error message if an error while parsing ocurred.
         * @return the error message if one.
         */
        QString getErrorMessage();
private:
        QString m_errMessage;   /// stores a error message if an error ocurred while parsing
        QScopedPointer<CASParser::Interpreter> m_i;        ///< stores an interpreter
};

#endif // EGCKERNELPARSER_H
