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


EgcNodeVisitor::EgcNodeVisitor(EgcFormulaEntity& formula) : m_result{QString::null}, m_formula{&formula},
                                                            m_state{EgcIteratorState::LeftIteration},
                                                            m_childIndex{0}
{
        m_stack.clear();
        m_argRegex = QRegularExpression("%[0-9]{1,2}");
        m_argRegex.optimize();
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

void EgcNodeVisitor::assembleResult(QString formatString, EgcNode* node)
{
        QString result = formatString;
        quint32 nrArguments;
        int i;

        if (!node)
                return;
                
        if (!node->isContainer())
                return;

        nrArguments = static_cast<EgcContainerNode*>(node)->getNumberChildNodes();

        if (nrArguments == 0)
                return;

        if (m_suppressList.contains(node)) {
                if (nrArguments) {
                        if (    node->isUnaryNode() 
                             || (node->isFlexNode() && nrArguments == 1))
                                deleteFromStack(nrArguments - 1);
                        else
                                deleteFromStack(nrArguments);
                }
                return;
        }

        QVector<QString> args(nrArguments);

        for (i = nrArguments - 1; i >= 0; i--) {
                if (!m_stack.isEmpty())
                        args[i] = m_stack.pop();
        }

        for (int i = 0; i < nrArguments; i++) {
                QRegularExpressionMatch match = m_argRegex.match(result);
                if (match.hasMatch() > 0)
                        result = result.arg(args[i]);
        }

        m_stack.push(result);
}

void EgcNodeVisitor::assembleResult(QString startString, QString seperationString, QString endString, EgcNode* node)
{
        QString result = startString;
        quint32 nrArguments;

        if (!node)
                return;
                
        if (!node->isContainer())
                return;

        nrArguments = static_cast<EgcContainerNode*>(node)->getNumberChildNodes();

        if (nrArguments == 0)
                return;

        if (m_suppressList.contains(node)) {
                if (nrArguments) {
                        if (    node->isUnaryNode() 
                             || (node->isFlexNode() && nrArguments == 1))
                                deleteFromStack(nrArguments - 1);
                        else
                                deleteFromStack(nrArguments);
                }
                return;
        }

        QVector<QString> args(nrArguments);

        for (int i = nrArguments - 1; i >= 0; i--) {
                if (!m_stack.isEmpty())
                        args[i] = m_stack.pop();
        }

        for (int i = 0; i < nrArguments; i++) {
                result += args[i];
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
