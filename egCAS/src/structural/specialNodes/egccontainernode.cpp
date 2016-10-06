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

#include <QScopedPointer>
#include "egccontainernode.h"

EgcContainerNode::EgcContainerNode()
{
}

EgcContainerNode::EgcContainerNode(const EgcContainerNode& orig)
{
        m_parent = nullptr;
}

EgcContainerNode::~EgcContainerNode()
{
}

bool EgcContainerNode::isContainer(void) const
{
        return true;
}

void EgcContainerNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        (void) old_child;
        (void) new_child;
}

EgcNode* EgcContainerNode::getChild(quint32 index) const
{
        (void) index;

        return nullptr;
}

bool EgcContainerNode::setChild(quint32 index, EgcNode& expression)
{
        (void) index;
        (void) expression;
}

quint32 EgcContainerNode::getNumberChildNodes(void) const
{
        return 0;
}

bool EgcContainerNode::isFirstChild(EgcNode &child) const
{
        (void) child;

        return false;
}

bool EgcContainerNode::isLastChild(EgcNode &child) const
{
        (void) child;

        return false;
}

bool EgcContainerNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        (void) child;
        (void) index;

        return false;
}

bool EgcContainerNode::transferProperties(EgcContainerNode &from)
{
        bool retval = false;
        quint32 nrChildNodes = this->getNumberChildNodes();
        quint32 nrChildsFrom = from.getNumberChildNodes();
        quint32 i;

        if (!this->isFlexNode()) {
                if (from.getNumberChildNodes() != nrChildNodes)
                        return false;
        }

        //test if all childs are null
        EgcNode* nd;
        for (i = 0; i < nrChildNodes; i++) {
                nd = this->getChild(i);
                //check also if there is a reordering protector
                if (nd != nullptr) {
                        if (nd->getNodeType() != EgcNodeType::ParenthesisNode/*EgcNodeType::ReorderingProtectorNode*/) {
                                return false;
                        } else {
                                if (static_cast<EgcContainerNode*>(nd)->getChild(0) != nullptr)
                                        return false;
                        }
                }
        }

        EgcNode* child;
        QScopedPointer<EgcNode> tempChild;
        for (i = 0; i < nrChildsFrom; i++) {
                child = from.getChild(i);
                if (child) {
                        tempChild.reset(from.takeOwnership(*child));
                        this->setChild(i, *tempChild.take());
                }
        }
        m_parent = from.getParent();
        m_parent->adjustChildPointers(from, *this);
        from.m_parent = nullptr;
        retval = true;

        return retval;
}

bool EgcContainerNode::isOperation(void) const
{
        return true;
}

bool EgcContainerNode::hasSubNode(const EgcNode& node, quint32 &index) const
{
        EgcNode* parent = nullptr;
        EgcNode* node_l = const_cast<EgcNode*>(&node);

        while (parent != this) {
                parent = node_l->getParent();
                if (parent) {
                        if (parent != this)
                                node_l = parent;
                } else {
                        return false;
                }
        }

        const EgcContainerNode* container = static_cast<const EgcContainerNode*>(parent);
        (void) container->getIndexOfChild(*node_l, index);

        return true;
}

bool EgcContainerNode::isLeafContainer(void) const
{
        return false;
}

qint32 EgcContainerNode::bindingPower(void) const
{
        if (isOperation()) {
                qint32 power = getBindingPower();
                if (power == -1)
                        qDebug("no binding power defined for this operation, please define one.");
                return power;
        } else {
                return -1;
        }
}

qint32 EgcContainerNode::getBindingPower(void) const
{
        return -1;
}

bool EgcContainerNode::isLeftAssociative(void) const
{
        return true;
}
