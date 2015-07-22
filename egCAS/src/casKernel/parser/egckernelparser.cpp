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

#include <iostream>
#include <string>
#include <sstream>
#include <QString>
#include <QStringBuilder>
#include "scanner.h"
#include "parser.hpp"
#include "interpreter.h"
#include "egckernelparser.h"

using namespace CASParser;
using namespace std;


EgcKernelParser::EgcKernelParser()
{
}

EgcNode* EgcKernelParser::parseKernelOutput(const QString& strToParse)
{
        Interpreter i;
        stringstream ss;

        ss << strToParse.toStdString();
        i.switchInputStream(&ss);
        try {
                if (i.parse()) {
                        m_errMessage = "common unspecified error while parsing input";
                        return nullptr;
                }
        } catch (const MaximaParser::syntax_error& e) {
                m_errMessage = "parsing error: " % QString(e.what()) %
                               QString(" at position: %1, %2").arg(e.location.begin.line).arg(e.location.begin.column);
                return nullptr;
        } catch (runtime_error& e) {
                m_errMessage = "runtime error: " % QString(e.what()) % " : Not enough memory?";
                return nullptr;
        } catch (...) {
                m_errMessage = "common unspecified exception while parsing input";
                return nullptr;
        }

        return i.getRootNode();
}

QString EgcKernelParser::getErrorMessage()
{
        return m_errMessage;
}
