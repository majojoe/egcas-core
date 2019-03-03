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
#include "formulainterpreter.h"
#include "egckernelparser.h"
#include "entities/formulamodificator.h"

//#include <cxxabi.h>  //only for getting exception type

using namespace std;


EgcKernelParser::EgcKernelParser() : m_i{new FormulaInterpreter()}
{
}

EgcKernelParser::~EgcKernelParser()
{

}

EgcNode* EgcKernelParser::parseKernelOutput(const QString& strToParse)
{
        stringstream ss;

        ss << strToParse.toStdString();
        try {
                if (m_i->parse(ss, true)) {
                        m_errMessage = "common unspecified error while parsing input";
                        return nullptr;
                }
        } catch (runtime_error& e) {
                m_errMessage = "runtime error: " % QString(e.what()) % " : Not enough memory?";
                return nullptr;
        } catch (antlr4::RecognitionException& e) {
                m_errMessage = "recognition error: " % QString(e.what());
                return nullptr;
        } catch (...) {
                m_errMessage = "common unspecified exception while parsing input";
                return nullptr;
        }

        if (m_i->isParsingErrorOccurred()) {
                m_errMessage = m_i->getErrorMessage();
                return nullptr;
        }

        return m_i->getRootNode();
}

QString EgcKernelParser::determineColumnOfCursor(QString strToParse, int &column, bool &isOnRightSide)
{
        QString str = strToParse;
        column = -1;
        int lind = strToParse.indexOf("_<L");
        int rind = strToParse.indexOf("_>R");
        if (lind != -1) { //remove left pointer from string
                str = str.remove(lind, 3);
                column = lind - 1;
                isOnRightSide = true;
        } else if (rind != -1) { //remove right pointer from string
                str = str.remove(rind, 3);
                column = rind;
                isOnRightSide = false;
        }

        return str;
}

EgcNode* EgcKernelParser::restructureFormula(const QString& strToParse, NodeIterReStructData& iterData, int* errCode)
{
        stringstream ss;
        *errCode = 0;
        int column;
        bool isOnRightSide;

        QString str = determineColumnOfCursor(strToParse, column, isOnRightSide);
        ss << str.toStdString();
        if (column != -1) {
                m_i->setCursorColumn(static_cast<quint32>(column));
                m_i->setSideOfColumn(isOnRightSide);
        }
        try {
                if (m_i->parse(ss)) {
                        //common unspecified error while parsing input
                        *errCode = 1;
                        return nullptr;
                }
        } catch (runtime_error& e) {
                (void) e;
                //runtime error: e.what(): Not enough memory?
                *errCode = 3;
                return nullptr;
        } catch (antlr4::RecognitionException& e) {
                (void) e;
                *errCode = 2;
                return nullptr;
        } catch (...) {
                //common unspecified exception while parsing input
                //int status;
                //const char* tmp = abi::__cxa_demangle(abi::__cxa_current_exception_type()->name(), 0, 0, &status);
                *errCode = 4;
                return nullptr;
        }

        iterData.m_node = m_i->getIteratorNode(0);
        iterData.m_isLeftPointer = isOnRightSide;
        iterData.m_offset = m_i->getOffset();
        iterData.m_cursorColumn = m_i->getCursorColumn();

        return m_i->getRootNode();
}

QString EgcKernelParser::getErrorMessage()
{
        QString temp = m_errMessage;
        m_errMessage = QString::null;

        return temp;
}
