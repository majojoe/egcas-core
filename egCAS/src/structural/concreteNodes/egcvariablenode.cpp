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

#include <QString>
#include <QStringBuilder>
#include <QRegularExpression>
#include "egcalnumnode.h"
#include "egcvariablenode.h"
#include "structural/specialNodes/egcemptynode.h"
#include "utils/egcutfcodepoint.h"


//ATTENTION: as of now egCAS and even Qt does not support non bmp characters (unicode caracters > 0xFFFF)

QRegularExpression EgcVariableNode::s_varSubSeparator = QRegularExpression("(.*[^_]+)_1([^_]+.*)");
bool EgcVariableNode::s_initializeRegex = true;

EgcVariableNode::EgcVariableNode() 
{
        //optimize all the regexes
        if (s_initializeRegex) {
                s_initializeRegex = false;
                s_varSubSeparator.optimize();
        }
        
        QScopedPointer<EgcAlnumNode> val(new EgcAlnumNode());
        setChild(0, *val.take());
        QScopedPointer<EgcAlnumNode> sub(new EgcAlnumNode(true));
        setChild(1, *sub.take());
        EgcAlnumNode* alnum = static_cast<EgcAlnumNode*>(m_leftChild.data());
        alnum->setValue(QString::null);
        alnum = static_cast<EgcAlnumNode*>(m_rightChild.data());
        alnum->setValue(QString::null);
}

EgcVariableNode::~EgcVariableNode()
{

}

void EgcVariableNode::setValue(const QString& varName, const QString& subscript = QString::null)
{
        EgcAlnumNode *alnum;
        if (m_leftChild->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_leftChild.data());
                alnum->setValue(varName);
        }

        if (m_rightChild->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_rightChild.data());
                alnum->setValue(subscript);
        }
}

void EgcVariableNode::setStuffedVar(const QString& varName)
{
        QString tmp = varName;
        QString value;
        QString subscript;

        /*the regexes are copied from static expressions since copying is cheap but we don't need to build up the
        regexes on the stack*/
        QRegularExpressionMatch regexMatch = s_varSubSeparator.match(tmp);
        if (regexMatch.hasMatch()) {
                value = regexMatch.captured(1);
                subscript = regexMatch.captured(2);

                EgcAlnumNode *alnum;
                if (m_leftChild->getNodeType() == EgcNodeType::AlnumNode) {
                        alnum = static_cast<EgcAlnumNode*>(m_leftChild.data());
                        alnum->setStuffedValue(value);
                }

                if (m_rightChild->getNodeType() == EgcNodeType::AlnumNode) {
                        alnum = static_cast<EgcAlnumNode*>(m_rightChild.data());
                        alnum->setStuffedValue(subscript);
                }
        } else {
                EgcAlnumNode *alnum;
                if (m_leftChild->getNodeType() == EgcNodeType::AlnumNode) {
                        alnum = static_cast<EgcAlnumNode*>(m_leftChild.data());
                        alnum->setStuffedValue(tmp);
                }

                if (m_rightChild->getNodeType() == EgcNodeType::AlnumNode) {
                        alnum = static_cast<EgcAlnumNode*>(m_rightChild.data());
                        alnum->setStuffedValue(QString::null);
                }
        }
}

QString EgcVariableNode::getValue(void) const
{
        QString retval = QString::null;

        EgcAlnumNode *alnum;
        if (m_leftChild->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_leftChild.data());
                retval = alnum->getValue();
        }

        return retval;
}

QString EgcVariableNode::getSubscript(void) const
{
        QString retval = QString::null;

        EgcAlnumNode *alnum;
        if (m_rightChild->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_rightChild.data());
                retval = alnum->getValue();
        }

        return retval;
}

QString EgcVariableNode::getStuffedVar(void)
{
        QString var;
        QString sub;

        EgcAlnumNode *alnum;
        if (m_leftChild->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_leftChild.data());
                var = alnum->getStuffedValue();
        }

        if (m_rightChild->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_rightChild.data());
                sub = alnum->getStuffedValue();
        }

        QString tmp;
        if (sub.isEmpty())
                tmp = var;
        else
                tmp = var % "_1" % sub;

        return tmp;
}

bool EgcVariableNode::valid(void)
{
        QString val;
        EgcAlnumNode *alnum;

        if (m_leftChild->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_leftChild.data());
                val = alnum->getValue();
        }

        if (val.isEmpty())
                return false;
        else
                return true;
}

bool EgcVariableNode::operator==(const EgcNode& node) const
{
        bool retval = false;
        QString val;
        QString sub;
        QString nodeVal;
        QString nodeSub;

        if (node.getNodeType() == EgcNodeType::VariableNode) {
                if (m_leftChild->getNodeType() == EgcNodeType::AlnumNode)
                        val = static_cast<EgcAlnumNode*>(m_leftChild.data())->getValue();

                if (m_leftChild->getNodeType() == EgcNodeType::EmptyNode)
                        val = static_cast<EgcEmptyNode*>(m_leftChild.data())->getValue();

                if (m_rightChild->getNodeType() == EgcNodeType::AlnumNode)
                        val = static_cast<EgcAlnumNode*>(m_rightChild.data())->getValue();

                if (m_rightChild->getNodeType() == EgcNodeType::EmptyNode)
                        val = static_cast<EgcEmptyNode*>(m_rightChild.data())->getValue();

                const EgcVariableNode& node = static_cast<const EgcVariableNode&>(node);
                EgcNode* nodeLeft = node.getChild(0);
                EgcNode* nodeRight = node.getChild(1);

                if (nodeLeft) {
                        if (nodeLeft->getNodeType() == EgcNodeType::AlnumNode)
                                val = static_cast<EgcAlnumNode*>(nodeLeft)->getValue();

                        if (nodeLeft->getNodeType() == EgcNodeType::EmptyNode)
                                val = static_cast<EgcEmptyNode*>(nodeLeft)->getValue();
                }

                if (nodeRight) {
                        if (nodeRight->getNodeType() == EgcNodeType::AlnumNode)
                                val = static_cast<EgcAlnumNode*>(nodeRight)->getValue();

                        if (nodeRight->getNodeType() == EgcNodeType::EmptyNode)
                                val = static_cast<EgcEmptyNode*>(nodeRight)->getValue();
                }

                if (    (val == nodeVal)
                     && (sub == nodeSub))
                        retval = true;
        }

        return retval;
}

bool EgcVariableNode::isOperation(void) const
{
        return false;
}
