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

#warning rebuild EgcVariableNode so, that is is based on EgcNode (no container class like EgcNumber)


//ATTENTION: as of now egCAS and even Qt does not support non bmp characters (unicode caracters > 0xFFFF)

QRegularExpression EgcVariableNode::s_varSubSeparator = QRegularExpression("(.*[^_]+)"
                                                                           % EgcVariableNode::getStuffedVarSeparator()
                                                                           % "(.+)");
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
        EgcAlnumNode* alnum = static_cast<EgcAlnumNode*>(m_childs.at(0));
        alnum->setValue(QString::null);
}

EgcVariableNode::~EgcVariableNode()
{

}

void EgcVariableNode::setValue(const QString& varName, const QString& subscript)
{
        if (m_childs.size() == 0)
                return;
        if (!m_childs.at(0))
                return;

        EgcAlnumNode *alnum;
        if (m_childs.at(0)->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_childs.at(0));
                alnum->setValue(varName);
        }

        if (subscript.isNull() && this->nrSubindexes() > 1)
                this->remove(1);
        if (!subscript.isNull() && this->nrSubindexes() > 1) {
                if (m_childs.at(1)) {
                        if (m_childs.at(1)->getNodeType() == EgcNodeType::EmptyNode)
                                this->remove(1);
                }
        }
        if (!subscript.isNull() && this->nrSubindexes() <= 1) {
                QScopedPointer<EgcAlnumNode> sub(new (std::nothrow) EgcAlnumNode(true));
                if (!sub.isNull())
                        this->insert(1, *sub.take());
        }

        if (m_childs.size() <= 1)
                return; //nothing any more to do
        if (!m_childs.at(1))
                return; //nothing any more to do

        if (m_childs.at(1)->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_childs.at(1));
                alnum->setValue(subscript);
        }
}

void EgcVariableNode::insertSubscript(void)
{
        if (m_childs.size() != 1)
                return;

        QScopedPointer<EgcAlnumNode> sub(new (std::nothrow) EgcEmptyNode());
        if (!sub.isNull())
                this->insert(1, *sub.take());
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
                if (m_childs.at(0)->getNodeType() == EgcNodeType::AlnumNode) {
                        alnum = static_cast<EgcAlnumNode*>(m_childs.at(0));
                        alnum->setStuffedValue(value);
                }

                if (this->nrSubindexes() > 1) {
                        if (m_childs.at(1)) {
                                if (m_childs.at(1)->getNodeType() == EgcNodeType::EmptyNode)
                                        this->remove(1);
                        }
                }

                if (this->nrSubindexes() <= 1) {
                        if (subscript == QString("_empty")) {
                                QScopedPointer<EgcEmptyNode> sub(new (std::nothrow) EgcEmptyNode());
                                if (!sub.isNull())
                                        this->insert(1, *sub.take());
                        } else {
                                QScopedPointer<EgcAlnumNode> sub(new (std::nothrow) EgcAlnumNode(true));
                                if (!sub.isNull())
                                        this->insert(1, *sub.take());
                        }
                }

                if (m_childs.size() <= 1)
                        return; //nothing any more to do
                if (!m_childs.at(1))
                        return; //nothing any more to do

                if (m_childs.at(1)->getNodeType() == EgcNodeType::AlnumNode) {
                        alnum = static_cast<EgcAlnumNode*>(m_childs.at(1));
                        alnum->setStuffedValue(subscript);
                }
        } else {
                EgcAlnumNode *alnum;
                if (m_childs.at(0)->getNodeType() == EgcNodeType::AlnumNode) {
                        alnum = static_cast<EgcAlnumNode*>(m_childs.at(0));
                        alnum->setStuffedValue(tmp);
                }

                if (this->nrSubindexes() > 1)
                        this->remove(1);
        }
}

QString EgcVariableNode::getValue(void) const
{
        QString retval = QString::null;

        if (m_childs.size() == 0)
                return retval;
        if (!m_childs.at(0))
                return retval;

        EgcAlnumNode *alnum;
        if (m_childs.at(0)->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_childs.at(0));
                retval = alnum->getValue();
        }

        return retval;
}

QString EgcVariableNode::getSubscript(void) const
{
        QString retval = QString::null;

        if (m_childs.size() <= 1)
                return retval; //nothing any more to do
        if (!m_childs.at(1))
                return retval; //nothing any more to do

        EgcAlnumNode *alnum;
        if (m_childs.at(1)->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_childs.at(1));
                retval = alnum->getValue();
        }

        return retval;
}

QString EgcVariableNode::getStuffedVar(void)
{
        QString var;
        QString sub;

        EgcAlnumNode *alnum;
        if (m_childs.at(0)->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_childs.at(0));
                var = alnum->getStuffedValue();
        }

        if (m_childs.size() > 1) {
                if (m_childs.at(1)) {
                        if (m_childs.at(1)->getNodeType() == EgcNodeType::AlnumNode) {
                                alnum = static_cast<EgcAlnumNode*>(m_childs.at(1));
                                sub = alnum->getStuffedValue();
                        }
                }
        }

        QString tmp;
        if (sub.isEmpty())
                tmp = var;
        else
                tmp = var % getStuffedVarSeparator() % sub;

        return tmp;
}

QString EgcVariableNode::getStuffedVarSeparator(void)
{
        return "_1";
}

bool EgcVariableNode::valid(void)
{
        QString val;
        EgcAlnumNode *alnum;

        if (m_childs.at(0)->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_childs.at(0));
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
        QString val = QString::null;
        QString sub = QString::null;
        QString nodeVal = QString::null;
        QString nodeSub = QString::null;

        if (node.getNodeType() == EgcNodeType::VariableNode) {
                if (m_childs.at(0)->getNodeType() == EgcNodeType::AlnumNode)
                        val = static_cast<EgcAlnumNode*>(m_childs.at(0))->getValue();

                if (m_childs.size() > 1) {
                        if (m_childs.at(1)) {
                                if (m_childs.at(1)->getNodeType() == EgcNodeType::AlnumNode)
                                        sub = static_cast<EgcAlnumNode*>(m_childs.at(1))->getValue();
                        }
                }

                if (node.getNodeType() != getNodeType())
                        return false;

                const EgcVariableNode& var_node = static_cast<const EgcVariableNode&>(node);
                EgcNode* nodeLeft = var_node.getChild(0);

                if (var_node.nrSubindexes() != nrSubindexes())
                        return false;

                EgcNode* nodeRight = var_node.getChild(1);

                if (nodeLeft) {
                        if (nodeLeft->getNodeType() == EgcNodeType::AlnumNode)
                                nodeVal = static_cast<EgcAlnumNode*>(nodeLeft)->getValue();
                }

                if (nodeRight) {
                        if (nodeRight->getNodeType() == EgcNodeType::AlnumNode)
                                nodeSub = static_cast<EgcAlnumNode*>(nodeRight)->getValue();
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

