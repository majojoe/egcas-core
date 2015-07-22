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

#ifndef EGCKERNELCONN_H
#define EGCKERNELCONN_H

#include <QString>
#include <QProcess>
#include <QRegularExpression>
#include <QScopedPointer>

/**
 * @brief The EgcNodeType enum is a enum to differentiate the different node types
 */
enum class EgcKernelStart
{
        beforeStart = 0, Starting, Started
};

/**
 * @brief The EgcKernelConn class is a base class for connections to different CAS kernels
 */
class EgcKernelConn: public QObject
{
        Q_OBJECT
public:
        /**
         * @brief EgcKernelConn creates a connection to a CAS kernel. This is done by starting the kernel.
         * It waits for commands then and will bring back the results.
         * @param executeCmd the path to the maxima executable including arguments.
         */
        EgcKernelConn(QString executeCmd, QObject *parent = 0);
        ///destructor
        virtual ~EgcKernelConn();
        /**
         * @brief sendCommand send a command to the kernel
         * @param cmd command to be sent to the kernel
         */
        virtual void sendCommand(QString cmd);
        /**
         * @brief quit quit kernel subprocess
         *
         * To avoid just killing the kernel you should call quit and wait for the kernelTerminated(); signal
         */
        virtual void quit(void) = 0;
        /**
         * @brief reset resets all variables assinged in the CAS kernel.
         */
        virtual void reset() = 0;

signals:
        /**
         * @brief resultReceived
         * @param result
         */
        void resultReceived(QString result);
        void errorReceived(QString errorMsg);
        void kernelStarted(void);
        void kernelTerminated(void);
        void kernelErrorOccurred(QProcess::ProcessError error);
protected slots:
        virtual void stdOutput(void) = 0;
        virtual void errorOutput(void);
        void kernelTerm(void);
        void kernelError(QProcess::ProcessError error);

protected:
        QString m_result;                               ///< stores the result until the result is complete
        QString m_error;                                ///< stores the error message until the message is complete
        QScopedPointer<QProcess> m_casKernelProcess;    ///< pointer to the kernel QProcess object
        EgcKernelStart m_startState;                    ///< marks the starting state of the CAS kernel
        QRegularExpression m_startRegex;                ///< regex for filtering kernel output at startup
        QRegularExpression m_errRegex;                  ///< regex for filtering kernel errors
        QRegularExpression m_regex;                     ///< regex for filtering regular kernel output
};

#endif // EGCKERNELCONN_H
