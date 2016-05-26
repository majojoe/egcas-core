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

#include "../entities/egcformulaentity.h"
#include "egcnodevisitor.h"
#include "../egcnodes.h"

QRegularExpression EgcNodeVisitor::s_argRegex = QRegularExpression("%[0-9]{1,2}");
bool EgcNodeVisitor::s_regexInitalized = false;


EgcNodeVisitor::EgcNodeVisitor(EgcFormulaEntity& formula) : m_result{QString::null}, m_formula{&formula},
                                                            m_state{EgcIteratorState::LeftIteration},
                                                            m_childIndex{0}
{
        m_stack.clear();
        if (!s_regexInitalized) {
                s_regexInitalized = true;
                s_argRegex.optimize();
        }
        m_suppressList.clear();
}

QString EgcNodeVisitor::getResult(void)
{
        EgcNodeIterator iter(*m_formula);
        EgcNode *node;
        EgcNode *previousChildNode;
        QString result;

        //clear stack
        m_stack.clear();

        while(iter.hasNext()) {
                previousChildNode = &iter.peekPrevious();
                node = &iter.next();
                m_state = iter.getLastState();
                if (node->isContainer()) {
                        if (m_state == EgcIteratorState::LeftIteration) {
                                m_childIndex = 0;
                        } else if (   m_state == EgcIteratorState::RightIteration) {
                                m_childIndex = static_cast<EgcContainerNode*>(node)->getNumberChildNodes();
                        } else {
                                quint32 ind;
                                if (static_cast<EgcContainerNode*>(node)->getIndexOfChild(*previousChildNode, ind))
                                      m_childIndex = ind;
                                else
                                      m_childIndex = 0;
                        }
                } else {
                        m_childIndex = 0;
                }
                node->accept(this);
                result += m_result;
                m_result = QString::null;
        };

        //add the result from the stack
        if (!m_stack.isEmpty())
                result += m_stack.pop();

        return result;
}

QVector<QString> EgcNodeVisitor::getAssembleArguments(EgcNode* node)
{
        quint32 nrArguments;
        QVector<QString> args;

        if (!node)
                args;
                
        if (!node->isContainer())
                args;

        nrArguments = static_cast<EgcContainerNode*>(node)->getNumberChildNodes();

        if (nrArguments == 0)
                args;

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

void EgcNodeVisitor::assembleResult(QString formatString, EgcNode* node)
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

        if (!m_suppressList.contains(node))
                m_stack.push(result);
}

void EgcNodeVisitor::assembleResult(QString startString, QString seperationString, QString endString, EgcNode* node)
{
        QString result = startString;
        quint32 nrArguments = 0;        

        QVector<QString> args = getAssembleArguments(node);
        nrArguments = args.size();
        if (nrArguments == 0)
                return;
        
        for (int i = 0; i < nrArguments; i++) {
                result += args.at(i);
                if (i != nrArguments - 1)
                        result += seperationString;
                else
                        result += endString;
        }

        if (!m_suppressList.contains(node))
                m_stack.push(result);
}

void EgcNodeVisitor::deleteFromStack(int nrStackObjects)
{
        int i;

        for (i = 0; i < nrStackObjects; i++) {
                if (!m_stack.isEmpty())
                        (void) m_stack.pop();
        }
}

void EgcNodeVisitor::pushToStack(QString str, EgcNode* node)
{
        if (m_suppressList.contains(node))
                m_stack.push(QString(""));
        else
                m_stack.push(str);
}

void EgcNodeVisitor::suppressChild(const EgcNode* node, quint32 index)
{
        EgcNode* chldNode = getChildToSuppress(node, index);
        if (chldNode) {
                m_suppressList.insert(chldNode);
        }
}

void EgcNodeVisitor::suppressThis(const EgcNode* node)
{
        if (node)
                m_suppressList.insert(const_cast<EgcNode*>(node));
}

EgcNode* EgcNodeVisitor::getChildToSuppress(const EgcNode* node, quint32 index)
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
