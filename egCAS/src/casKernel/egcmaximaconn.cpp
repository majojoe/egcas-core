/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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
#include "egcmaximaconn.h"

QString EgcMaximaConn::s_startupConfig = QString("set_display(none);display2d:false;");

EgcMaximaConn::EgcMaximaConn(QString executeMaximaCmd, QObject *parent) : EgcKernelConn(executeMaximaCmd, parent)
{
        //information to remove from error messages of maxima
        //use the vector below to add more messages that we don't want to show to the user.
        QVector<QString> errorMsgFilter;
        errorMsgFilter.append("To debug this try:");
        errorMsgFilter.append("Maxima ");

        //there are some words That should be replaced in error messages
        m_wordsToReplace.insert("Maxima", "The CAS kernel");
        m_wordsToReplace.insert("maxima", "the CAS kernel");


        QString regexFilterStr;
        QString str;
        foreach (str, errorMsgFilter) {
                regexFilterStr += QRegularExpression::escape(str);
                regexFilterStr += QString("|");
        }
        if (regexFilterStr.endsWith('|'))
                regexFilterStr.remove(-1, 1);

        m_startRegex = QRegularExpression(".*\n\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
        m_errRegex = QRegularExpression("(.*)\n\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
        m_regex = QRegularExpression("\\(%o[0-9]+\\)(.*)\n\\(%i[0-9]+\\)", QRegularExpression::DotMatchesEverythingOption);
        m_errUnwantedRegex = QRegularExpression("(.*)(" + regexFilterStr + ").*", QRegularExpression::DotMatchesEverythingOption);
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
                        emit resultReceived(match.captured(1).trimmed().simplified());
                        m_result.clear();
                } else {
                        match = m_errRegex.match(m_result);
                        if (match.hasMatch()) {
                                QString errorString = match.captured(1).trimmed();
                                if (m_errUnwantedRegex.match(errorString).hasMatch())
                                        errorString = m_errUnwantedRegex.match(errorString).captured(1);

                                QMapIterator<QString, QString> i(m_wordsToReplace);
                                while (i.hasNext()) {
                                    i.next();
                                    if (errorString.contains(i.key()))
                                            errorString.replace(i.key(), i.value());
                                }

                                emit errorReceived(errorString);
                                m_result.clear();
                        } else {  //this seems to be a normal result, but only a part of it
#warning start a timer here. If the operation does not complete within a specific amount of time -> error in parsing...
                        }
                }
                break;
        }
}

void EgcMaximaConn::quit(void)
{
        this->sendCommand("quit();");
}

void EgcMaximaConn::reset(void)
{
        this->sendCommand("kill(all);");
}

