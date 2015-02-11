#include "egcmaximaconn.h"

QString EgcMaximaConn::s_startupConfig = QString("set_display(none);display2d=false;");

EgcMaximaConn::EgcMaximaConn(QString executeMaximaCmd, QObject *parent) : EgcKernelConn(executeMaximaCmd, parent)
{
        m_startRegex = QRegularExpression(".*\n\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
        m_regex = QRegularExpression("\\(%o[0-9]+\\)(.*)\n\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
}

EgcMaximaConn::~EgcMaximaConn()
{
        quit();
}

void EgcMaximaConn::sendCommand(QString cmd)
{
        cmd += "\n";
        m_casKernelProcess->write(cmd.toUtf8());
}

void EgcMaximaConn::stdOutput(void)
{
        QByteArray temp = m_casKernelProcess->readAllStandardOutput();
        m_result += temp;
        QRegularExpressionMatch match;
        switch (m_startState) {
        case EgcKernelStart::beforeStart:
                match = m_startRegex.match(m_result);
                if (match.hasMatch()) {
                        m_result.clear();
                        sendCommand(s_startupConfig);
                        m_startState = EgcKernelStart::Starting;
                }
                break;
        case EgcKernelStart::Starting:
                match = m_startRegex.match(m_result);
                if (match.hasMatch()) {
                        emit kernelStarted();
                        m_result.clear();
                        m_startState = EgcKernelStart::Started;
                }
                break;
        default:
                match = m_regex.match(m_result);
                if (match.hasMatch()) {
                        emit resultReceived(match.captured(1).trimmed());
                        m_result.clear();
                } else {
#warning start a timer here. If the operation doesn't complete within a specific amount of time -> error in parsing...
                }
                break;
        }
}

void EgcMaximaConn::quit(void)
{
        this->sendCommand("quit();");
}
