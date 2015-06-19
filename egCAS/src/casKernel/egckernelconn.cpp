#include "egckernelconn.h"

EgcKernelConn::EgcKernelConn(QString executeCmd, QObject *parent) : QObject(parent), m_result(QString()),
                                                                          m_error(QString()),
                                                                          m_startState(EgcKernelStart::beforeStart)
{
        m_casKernelProcess.reset(new QProcess());
        m_casKernelProcess->start(executeCmd);

        /* show output */
        connect(m_casKernelProcess.data(), SIGNAL(readyReadStandardOutput()),this, SLOT(stdOutput()) );
        connect(m_casKernelProcess.data(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutput()) );
        connect(m_casKernelProcess.data(), SIGNAL(error(QProcess::ProcessError)), this, SLOT(kernelError(QProcess::ProcessError)) );
        connect(m_casKernelProcess.data(), SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(kernelTerm()) );
}

EgcKernelConn::~EgcKernelConn()
{
}

void EgcKernelConn::sendCommand(QString cmd)
{
        m_casKernelProcess->write(cmd.toUtf8());
}

void EgcKernelConn::errorOutput(void)
{
        QByteArray temp = m_casKernelProcess->readAllStandardError();
        qDebug("CAS kernel has thrown an error: %s\n", qPrintable(temp));
}

void EgcKernelConn::kernelTerm(void)
{
        emit kernelTerminated();
}

void EgcKernelConn::kernelError(QProcess::ProcessError error)
{
        emit kernelErrorOccurred(error);
}

