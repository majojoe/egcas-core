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

#ifndef EGCMAXIMACONN_H
#define EGCMAXIMACONN_H

#include <QString>
#include <QProcess>
#include <QRegularExpression>
#include <QMap>
#include <QTimer>
#include "egckernelconn.h"


class EgcMaximaConn : public EgcKernelConn
{
        Q_OBJECT
public:
        /**
         * @brief EgcMaximaConn creates a connection to maxima CAS kernel. This is done by starting the maxima kernel.
         * It waits for commands then and will bring back the results.
         * @param executeMaximaCmd the path to the maxima executable including arguments.
         */
        EgcMaximaConn(QObject *parent = 0);
        ///destructor
        virtual ~EgcMaximaConn();
        /**
         * @brief sendCommand send a command to maxima
         * @param cmd command to be sent to the kernel
         */
        void sendCommand(QString cmd);
        /**
         * @brief quit quit maxima subprocess
         */
        void quit(void);
        /**
         * @brief reset resets all variables assinged in the CAS kernel.
         */
        virtual void reset();

protected slots:
        /**
         * @brief stdOutput some sort of output from the cas kernel received
         */
        virtual void stdOutput(void);
        /**
         * @brief casKernelTimeoutError a cas kernel timeout error occurred
         */
        void casKernelTimeoutError(void);
        /**
         * @brief errorOutput overrride the error output
         */
        virtual void errorOutput(void) override;
protected:
        /**
         * @brief clearKernelOutQueue clears the output queue of the kernel
         */
        void clearKernelOutQueue(void);
        /**
         * @brief findMaximaExecutable finds the maxima executable
         * @return the found maxima executable
         */
        QString findMaximaExecutable(void);

        static QString s_startupConfig;         ///< startup configuration for CAS kernel
        QRegularExpression m_errUnwantedRegex;  ///< regex for filtering kernel unwanted information from error message
        QMap<QString, QString> m_wordsToReplace;///< words that schould be replaced
        QTimer* m_timer;                        ///< a timer to be able to fire a cas kernel reset if a error condition exists
};

#endif // EGCMAXIMACONN_H
