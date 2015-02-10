#include "egcmaximaconn.h"

EgcMaximaConn::EgcMaximaConn(QString executeMaximaCmd, QObject *parent) : QObject(parent), m_result(QString()),
                                                                          m_error(QString())
{
        m_regex = QRegularExpression("\\(%o[0-9]+\\).*\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
        m_casKernelProcess = new QProcess();
        m_casKernelProcess->start(executeMaximaCmd);

        /* show output */
        connect(m_casKernelProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(stdOutput()) );
        connect(m_casKernelProcess, SIGNAL(readyReadStandardError()), this, SLOT(errorOutput()) );
        connect(m_casKernelProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(kernelError(QProcess::ProcessError)) );
        connect(m_casKernelProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(kernelTerm()) );
        connect(m_casKernelProcess, SIGNAL(started()), this, SLOT(kernelStart()) );
}

EgcMaximaConn::~EgcMaximaConn()
{
        delete m_casKernelProcess;
}

void EgcMaximaConn::sendCommand(QString cmd)
{
        m_casKernelProcess->write(cmd.toUtf8());
}

void EgcMaximaConn::stdOutput(void)
{
        QByteArray temp = m_casKernelProcess->readAllStandardOutput();
        m_result += temp;
        if (m_regex.match(m_result).hasMatch()) {
                emit resultReceived(m_result);
                m_result.clear();
        } else {
#warning start a timer here. If the operation doesn't complete within a specific amount of time -> error in parsing...
        }
}

void EgcMaximaConn::errorOutput(void)
{
        QByteArray temp = m_casKernelProcess->readAllStandardError();
        m_error += temp;
        if (m_regex.match(m_error).hasMatch()) {
                emit errorReceived(m_error);
                m_error.clear();
        } else {
#warning start a timer here. If the operation doesn't complete within a specific amount of time -> error in parsing...
        }

}

void EgcMaximaConn::kernelStart(void)
{
        emit kernelStarted();
}

void EgcMaximaConn::kernelTerm(void)
{
        emit kernelTerminated();
}

void EgcMaximaConn::kernelError(QProcess::ProcessError error)
{
        emit kernelErrorOccurred(error);
}

void EgcMaximaConn::quit(void)
{
        this->sendCommand("quit();");
}
