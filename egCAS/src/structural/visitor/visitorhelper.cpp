/*
Copyright (c) 2014 - 2018, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
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

#include "../egcnodes.h"
#include "visitorhelper.h"

QRegularExpression VisitorHelper::s_argRegex = QRegularExpression("%[0-9]{1,2}");
bool VisitorHelper::s_regexInitalized = false;


VisitorHelper::VisitorHelper(EgcFormulaEntity& formula) : EgcNodeVisitor(formula)
{
        if (!s_regexInitalized) {
                s_regexInitalized = true;
                s_argRegex.optimize();
        }
        m_suppressList.clear();
}

VisitorHelper::~VisitorHelper()
{

}

QVector<QString> VisitorHelper::getAssembleArguments(EgcNode* node)
{
        quint32 nrArguments;
        QVector<QString> args;

        if (!node)
                return args;

        if (!node->isContainer())
                return args;

        nrArguments = static_cast<EgcContainerNode*>(node)->getNumberChildNodes();

        if (nrArguments == 0)
                return args;

        if (m_suppressList.contains(node)) {
                if (nrArguments) {
                        if (    node->isUnaryNode()
                             || (node->isFlexNode() && nrArguments == 1))
                                deleteFromStack(nrArguments - 1);
                        else
                                deleteFromStack(nrArguments);
                }
                return args;
        }

        args.resize(nrArguments);

        for (int i = nrArguments - 1; i >= 0; i--) {
                if (!m_stack.isEmpty())
                        args[i] = m_stack.pop();
        }

        return args;
}

void VisitorHelper::assembleResult(QString formatString, EgcNode* node)
{
        QString result = formatString;

        QVector<QString> args = getAssembleArguments(node);
        if (args.size() == 0)
                return;

        for (int i = 0; i < args.size(); i++) {
                QRegularExpressionMatch match = s_argRegex.match(result);
                if (match.hasMatch() > 0)
                        result = result.arg(args.at(i));
        }

        result = modifyNodeString(result, node);

        if (!m_suppressList.contains(node))
                m_stack.push(result);
}

void VisitorHelper::assembleResult(QString lStartString, QString rStartString, QString seperationString,
                                    QString endString, EgcNode* node)
{
        QString result = lStartString;
        quint32 nrArguments = 0;

        QVector<QString> args = getAssembleArguments(node);
        nrArguments = static_cast<quint32>(args.size());
        if (nrArguments == 0)
                return;

        result += args.at(0);
        result += rStartString;

        for (quint32 i = 1; i < nrArguments; i++) {
                result += args.at(static_cast<int>(i));
                if (i != nrArguments - 1)
                        result += seperationString;
        }

        result += endString;

        result = modifyNodeString(result, node);

        if (!m_suppressList.contains(node))
                m_stack.push(result);
}

void VisitorHelper::assembleResult(QString startString, QString seperationString, QString endString, EgcNode* node)
{
        QString result = startString;
        quint32 nrArguments = 0;

        QVector<QString> args = getAssembleArguments(node);
        nrArguments = static_cast<quint32>(args.size());
        if (nrArguments == 0)
                return;

        for (quint32 i = 0; i < nrArguments; i++) {
                result += args.at(static_cast<int>(i));
                if (i != nrArguments - 1)
                        result += seperationString;
                else
                        result += endString;
        }

        result = modifyNodeString(result, node);

        if (!m_suppressList.contains(node))
                m_stack.push(result);
}

void VisitorHelper::assembleResult(QString startString, QString seperationString, QString nthSeparationString,
                                   quint32 allNthSep, QString endString, EgcNode* node)
{
        QString result = startString;
        quint32 nrArguments = 0;

        QVector<QString> args = getAssembleArguments(node);
        nrArguments = static_cast<quint32>(args.size());
        if (nrArguments == 0)
                return;

        for (quint32 i = 0; i < nrArguments; i++) {
                result += args.at(static_cast<int>(i));
                if (i != nrArguments - 1) {
                        if ((i % allNthSep) == 0)
                                result += nthSeparationString;
                        else
                                result += seperationString;
                } else {
                        result += endString;
                }
        }

        result = modifyNodeString(result, node);

        if (!m_suppressList.contains(node))
                m_stack.push(result);
}

void VisitorHelper::deleteFromStack(quint32 nrStackObjects)
{
        quint32 i;

        for (i = 0; i < nrStackObjects; i++) {
                if (!m_stack.isEmpty())
                        (void) m_stack.pop();
        }
}

void VisitorHelper::pushToStack(QString str, EgcNode* node)
{
        str = modifyNodeString(str, node);

        if (m_suppressList.contains(node))
                m_stack.push(QString(""));
        else
                m_stack.push(str);
}

EgcNode* VisitorHelper::getChildToSuppress(const EgcNode* node, quint32 index)
{
        EgcNode* chldNode = nullptr;

        if (!node)
                return chldNode;

        if (!node->isContainer())
                return chldNode;

        const EgcContainerNode* container = static_cast<const EgcContainerNode*>(node);
        if (container->getChild(index)) {
                chldNode = container->getChild(index);
        }

        return chldNode;
}

QString& VisitorHelper::modifyNodeString(QString &nodeString, EgcNode* node)
{
        (void) node;
        return nodeString;
}
