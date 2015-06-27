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
