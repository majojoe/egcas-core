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

#include <QVector>
#include <QMap>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#ifdef DEBUG_MAXIMA_KERNEL
#include <QDebug>
#endif //DEBUG_MAXIMA_KERNEL
#include "egcmaximaconn.h"

QString EgcMaximaConn::s_startupConfig = QString("set_display(none)$display2d:false$");

EgcMaximaConn::EgcMaximaConn(QObject *parent) : EgcKernelConn{parent}, m_timer{new QTimer(this)}
{

        QString startCmd = findMaximaExecutable();
        if (startCmd.isEmpty()) {
#warning test emitting this error
                emit kernelErrorOccurred(QProcess::FailedToStart);
                return;
        }
        startKernel(startCmd);

        m_timer->setSingleShot(true);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(casKernelTimeoutError()));

        //information to remove from error messages of maxima
        //use the vector below to add more messages that we don't want to show to the user.
        QVector<QString> errorMsgFilter;
        errorMsgFilter.append("To debug this try:");
        errorMsgFilter.append("Maxima ");

        //there are some words That should be replaced in error messages
        m_wordsToReplace.insert("Maxima", "CAS kernel");
        m_wordsToReplace.insert("maxima", "CAS kernel");


        QString regexFilterStr;
        QString str;
        foreach (str, errorMsgFilter) {
                regexFilterStr += QRegularExpression::escape(str);
                regexFilterStr += QString("|");
        }
        if (regexFilterStr.endsWith('|'))
                regexFilterStr.remove(-1, 1);

        m_startRegex = QRegularExpression(".*\n\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
        m_startedRegex = QRegularExpression(".*\n\\(%i[2-9]{1}\\)", QRegularExpression::DotMatchesEverythingOption);
        m_errRegex = QRegularExpression("(.*)\n\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
        m_regex = QRegularExpression("\\(%o[0-9]+\\)(.*?)\n\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
        m_errUnwantedRegex = QRegularExpression("(.*)(" + regexFilterStr + ").*", QRegularExpression::DotMatchesEverythingOption);
        m_unwantedErrors = QRegularExpression("\\(%i1\\)");
        m_startRegex.optimize();
        m_errRegex.optimize();
        m_regex.optimize();
        m_errUnwantedRegex.optimize();
        m_unwantedErrors.optimize();
}

QString EgcMaximaConn::findMaximaExecutable(void)
{
        QString path = QFileInfo(QCoreApplication::applicationFilePath() ).absolutePath();
        QString startCmd = QString("");
        QString maximaExecutable = QString("egcas-maxima");
        QFileInfo maxima;
        //search for maxima binary in the same dir as egcas
        maxima.setFile(path, maximaExecutable);
        if (maxima.exists() && maxima.isExecutable()) {
                startCmd = maxima.absoluteFilePath();
        } else {
                //search for maxima binary in maxima installation path for development
                maxima.setFile(QDir::cleanPath(MAXIMA_BINARY_PATH) , maximaExecutable);
                if (maxima.exists() && maxima.isExecutable()) {
                        startCmd = maxima.absoluteFilePath();
                } else {
                        //third possibility is to check environment variables to find maxima executable
                        if (qEnvironmentVariableIsSet("MAXIMA_BINARY_PATH")) {
                                QString p = QString(qgetenv("MAXIMA_BINARY_PATH").constData());
                                maxima.setFile(p);
                                if (maxima.exists() && maxima.isExecutable())
                                        startCmd = maxima.absoluteFilePath();
                        }
                }
        }

        return startCmd;
}

EgcMaximaConn::~EgcMaximaConn()
{
        quit();
}

void EgcMaximaConn::clearKernelOutQueue(void)
{
        // read all rests of the output of the kernel
        (void) m_casKernelProcess->readAllStandardOutput();
}

void EgcMaximaConn::sendCommand(QString cmd)
{
        cmd += "\n";
#ifdef DEBUG_MAXIMA_KERNEL
        qDebug() << cmd.toUtf8();
#endif //DEBUG_MAXIMA_KERNEL
        clearKernelOutQueue();
        m_casKernelProcess->write(cmd.toUtf8());
}

void EgcMaximaConn::stdOutput(void)
{
        //stop timer if active
        if (m_timer->isActive())
                m_timer->stop();

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
                match = m_startedRegex.match(m_result);
                if (match.hasMatch()) {
                        emit kernelStarted();
                        m_result.clear();
                        m_startState = EgcKernelStart::Started;
                }
                break;
        default:
                match = m_regex.match(m_result);
                if (match.hasMatch()) {
                        QString result = match.captured(1).trimmed().simplified();
#ifdef DEBUG_MAXIMA_KERNEL
                        qDebug() << result;
#endif //DEBUG_MAXIMA_KERNEL
                        emit resultReceived(result);
                        m_result.clear();
                } else {
                        match = m_errRegex.match(m_result);
                        if (match.hasMatch()) {
                                QString errorString = match.captured(1).trimmed();
                                if (m_unwantedErrors.match(m_result).hasMatch()) { // this is an unwanted error
                                        m_result.clear();
                                        break;
                                }

                                if (m_errUnwantedRegex.match(errorString).hasMatch())
                                        errorString = m_errUnwantedRegex.match(errorString).captured(1);

                                QMapIterator<QString, QString> i(m_wordsToReplace);
                                while (i.hasNext()) {
                                    i.next();
                                    if (errorString.contains(i.key()))
                                            errorString.replace(i.key(), i.value());
                                }
#ifdef DEBUG_MAXIMA_KERNEL
                                qDebug() << errorString;
#endif //DEBUG_MAXIMA_KERNEL
                                emit errorReceived(errorString);
                                m_result.clear();
                        } else {  //this seems to be a normal result, but only a part of it
                                // if there is no complete result within 30s, anything must be wrong
                                m_timer->start(30000);
                        }
                }
                break;
        }
}

void EgcMaximaConn::quit(void)
{
        this->sendCommand("quit();");
        m_casKernelProcess->waitForFinished(1000);
}

void EgcMaximaConn::reset(void)
{
        clearKernelOutQueue();
        this->sendCommand("kill(all)$");
}

void EgcMaximaConn::casKernelTimeoutError(void)
{
#ifdef DEBUG_MAXIMA_KERNEL
                        qDebug() << m_result;
#endif //DEBUG_MAXIMA_KERNEL
        emit timeoutError();
}

void EgcMaximaConn::errorOutput(void)
{
        EgcKernelConn::errorOutput();
        if (m_timer->isActive())
                m_timer->stop();
}
