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

#ifdef DEBUG_MAXIMA_KERNEL
#include <QDebug>
#endif //#ifdef DEBUG_MAXIMA_KERNEL
#include "egckernelconn.h"

EgcKernelConn::EgcKernelConn(QObject *parent) : QObject(parent), m_result(QString()),
                                                                          m_error(QString()),
                                                                          m_startState(EgcKernelStart::beforeStart),
                                                                          m_executeCommand(QString(""))
{
}

EgcKernelConn::EgcKernelConn(QString executeCmd, QObject *parent) : QObject(parent), m_result(QString()),
                                                                          m_error(QString()),
                                                                          m_startState(EgcKernelStart::beforeStart),
                                                                          m_executeCommand(executeCmd)
{
        restart();
}

EgcKernelConn::~EgcKernelConn()
{
        
}

void EgcKernelConn::startKernel(QString binaryStartCmd)
{
        m_executeCommand = binaryStartCmd;
        restart();
}

void EgcKernelConn::sendCommand(QString cmd)
{
        m_casKernelProcess->write(cmd.toUtf8());
}

void EgcKernelConn::errorOutput(void)
{
        QByteArray temp = m_casKernelProcess->readAllStandardError();
#ifdef DEBUG_MAXIMA_KERNEL
                        qDebug() << "CAS kernel has thrown an error: " << temp;
#endif //DEBUG_MAXIMA_KERNEL
        emit errorReceived(temp);
}

void EgcKernelConn::kernelTerm(void)
{
        emit kernelTerminated();
}

void EgcKernelConn::kernelError(QProcess::ProcessError error)
{
        emit kernelErrorOccurred(error);
}

void EgcKernelConn::restart(void) 
{
        m_casKernelProcess.reset(new QProcess());
        m_casKernelProcess->start(m_executeCommand);

        /* show output */
        connect(m_casKernelProcess.data(), SIGNAL(readyReadStandardOutput()),this, SLOT(stdOutput()) );
        connect(m_casKernelProcess.data(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutput()) );
        connect(m_casKernelProcess.data(), SIGNAL(error(QProcess::ProcessError)), this, SLOT(kernelError(QProcess::ProcessError)) );
        connect(m_casKernelProcess.data(), SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(kernelTerm()) );
}
