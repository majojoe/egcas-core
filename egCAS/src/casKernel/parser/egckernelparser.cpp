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

bool EgcKernelParser::parseKernelOutput(const QString strToParse, EgcExpressionNode * result)
{
        Interpreter i;
        stringstream ss;

        ss << strToParse.toStdString();
        i.switchInputStream(&ss);
        try {
                if (!i.parse()) {
                        m_errMessage = "common unspecified error while parsing input";
                        result = nullptr;
                        return false;
                }
        } catch (const MaximaParser::syntax_error& e) {
                m_errMessage = "parsing error: " % QString(e.what()) %
                               QString(" at position: %1, %2").arg(e.location.begin.line).arg(e.location.begin.column);
                result = nullptr;
                return false;
        } catch (runtime_error& e) {
                m_errMessage = "runtime error: " % QString(e.what()) % " : Not enough memory?";
                result = nullptr;
                return false;
        } catch (...) {
                m_errMessage = "common unspecified exception while parsing input";
                result = nullptr;
                return false;
        }

        result = i.getBaseNode();
        return true;
}

QString EgcKernelParser::getErrorMessage()
{
        return m_errMessage;
}
