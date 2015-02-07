#include "egcmaximaconn.h"

EgcMaximaConn::EgcMaximaConn(QString executeMaximaCmd, QObject *parent) : QObject(parent), m_result(QString()),
                                                                          m_error(QString())
{
        m_regex = QRegExp(".*\\(%i[0-9]+\\)");
        m_casKernelProcess = new QProcess();
        m_casKernelProcess->start(executeMaximaCmd);

        /* show output */
        connect(m_casKernelProcess, SIGNAL(readyReadStandardOutput()),this, SLOT(stdOutput()) );
        connect(m_casKernelProcess, SIGNAL(readyReadStandardError()), this, SLOT(errorOutput()) );
        connect(m_casKernelProcess, SIGNAL(error()), this, SLOT(kernelError()) );
        connect(m_casKernelProcess, SIGNAL(finished()), this, SLOT(kernelTerm()) );
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
        if (m_regex.exactMatch(m_result)) {
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
        if (m_regex.exactMatch(m_error)) {
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
