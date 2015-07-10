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

#include "egcbinarynode.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcBinaryNode::EgcBinaryNode() : m_rightChild(nullptr), m_leftChild(nullptr)
{
}

EgcBinaryNode::EgcBinaryNode(const EgcBinaryNode& orig) : EgcContainerNode(orig)
{
        EgcNode *originalChildLeft = const_cast<EgcBinaryNode&>(orig).getChild(0);
        EgcNode *originalChildRight = const_cast<EgcBinaryNode&>(orig).getChild(1);
        if (originalChildLeft)
                m_leftChild.reset(originalChildLeft->copy());
        if (originalChildRight)
                m_rightChild.reset(originalChildRight->copy());

        //set the parents also
        if(m_leftChild)
                m_leftChild->provideParent(this);
        if(m_rightChild)
                m_rightChild->provideParent(this);
}

EgcBinaryNode::EgcBinaryNode(EgcBinaryNode&& orig) : EgcContainerNode(orig)
{
        EgcNode *originalChildLeft = const_cast<EgcBinaryNode&>(orig).getChild(0);
        EgcNode *originalChildRight = const_cast<EgcBinaryNode&>(orig).getChild(1);
        if (originalChildLeft)
                m_leftChild.reset(orig.takeOwnership(*originalChildLeft));
        if (originalChildRight)
                m_rightChild.reset(orig.takeOwnership(*originalChildRight));

        //set the parents also
        if(m_leftChild)
                m_leftChild->provideParent(this);
        if(m_rightChild)
                m_rightChild->provideParent(this);
}

EgcBinaryNode::~EgcBinaryNode()
{
}

EgcBinaryNode& EgcBinaryNode::operator=(const EgcBinaryNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //and create a new one
        EgcNode *originalChildLeft = rhs.getChild(0);
        EgcNode *originalChildRight = rhs.getChild(1);
        if (originalChildLeft)
                m_leftChild.reset(originalChildLeft->copy());
        if (originalChildRight)
                m_rightChild.reset(originalChildRight->copy());

        return *this;
}

EgcBinaryNode& EgcBinaryNode::operator=(EgcBinaryNode&& rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //and create a new one
        EgcNode *originalChildLeft = rhs.getChild(0);
        EgcNode *originalChildRight = rhs.getChild(1);
        if (originalChildLeft)
                m_leftChild.reset(rhs.takeOwnership(*originalChildLeft));
        if (originalChildRight)
                m_rightChild.reset(rhs.takeOwnership(*originalChildRight));

        return *this;
}

bool EgcBinaryNode::valid(void)
{
        if (m_leftChild && m_rightChild)
                if (m_leftChild->valid() && m_rightChild->valid())
                        return true;

        return false;
}

bool EgcBinaryNode::isBinaryNode(void) const
{
        return true;
}

void EgcBinaryNode::notifyContainerOnChildDeletion(EgcNode* child)
{
        if (m_leftChild.data() == child)
                m_leftChild.reset(nullptr);
        if (m_rightChild.data() == child)
                m_rightChild.reset(nullptr);
}

void EgcBinaryNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        if (m_leftChild.data() == &old_child) {
                (void) m_leftChild.take(); //do not delete the old child
                m_leftChild.reset(&new_child);
        } else if (m_rightChild.data() == &old_child) {
                (void) m_rightChild.take(); //do not delete the old child
                m_rightChild.reset(&new_child);
        }
}

EgcNode* EgcBinaryNode::takeOwnership(EgcNode &child)
{
        EgcNode* retval = nullptr;

        if (m_leftChild.data() == &child) {
                retval = m_leftChild.take();
                retval->provideParent(nullptr);
        } else if (m_rightChild.data() == &child) {
                retval = m_rightChild.take();
                retval->provideParent(nullptr);
        }

        return retval;
}

void EgcBinaryNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

EgcNode* EgcBinaryNode::getChild(quint32 index) const
{
        if (index == 0)
                return m_leftChild.data();
        else if (index == 1)
                return m_rightChild.data();
        else
                return nullptr;
}

bool EgcBinaryNode::setChild(quint32 index, const EgcNode& expression)
{
        bool retval = true;

        QScopedPointer<const EgcNode> expr(&expression);

        if (index == 0) {
                m_leftChild.reset(const_cast<EgcNode*>(expr.take()));

                if (m_leftChild)
                        m_leftChild->provideParent(this);
        } else if (index == 1) {
                m_rightChild.reset(const_cast<EgcNode*>(expr.take()));

                if (m_rightChild)
                        m_rightChild->provideParent(this);
        } else {
                retval = false;
        }

        return retval;
}

quint32 EgcBinaryNode::getNumberChildNodes(void) const
{
        return 2;
}

bool EgcBinaryNode::isFirstChild(EgcNode &child) const
{
        if (m_leftChild.data() == &child)
                return true;
        else
                return false;
}

bool EgcBinaryNode::isLastChild(EgcNode &child) const
{
        if (m_rightChild.data() == &child)
                return true;
        else
                return false;
}

EgcNode* EgcBinaryNode::incrementToNextChild(EgcNode &previousChild) const
{
        if (m_leftChild.data() == &previousChild || m_leftChild.data() == nullptr)
                return m_rightChild.data();
        else
                return nullptr;
}

EgcNode* EgcBinaryNode::decrementToPrevChild(EgcNode &previousChild) const
{
        if (m_rightChild.data() == &previousChild || m_rightChild.data() == nullptr)
                return m_leftChild.data();
        else
                return nullptr;
}

bool EgcBinaryNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        if (child.getParent() == this) {
                index = 0;
                if (&child == m_rightChild.data()) {
                        index = 1;
                }
                return true;
        }

        return false;
}

bool EgcBinaryNode::isEqual(EgcNode* node) const
{
        bool retval = false;

        if (!node)
                return false;

        if (node->isBinaryNode()) {
                if (m_leftChild->isEqual(static_cast<EgcBinaryNode*>(node)->getChild(0))) {
                     if (m_rightChild->isEqual(static_cast<EgcBinaryNode*>(node)->getChild(1)))
                             retval = true;
                 }
        }

        return retval;
}

bool EgcBinaryNode::operator==(const EgcNode& node) const
{
        bool retval = false;

        if (node.isBinaryNode()) {
                EgcNode* left = static_cast<const EgcBinaryNode&>(node).getChild(0);
                EgcNode* right = static_cast<const EgcBinaryNode&>(node).getChild(1);
                if (!m_leftChild.isNull() && !m_rightChild.isNull() && left && right ) {
                        if (*m_leftChild == *left) {
                                if (*m_rightChild == *right)
                                        retval = true;
                        }
                }
        }

        return retval;
}
