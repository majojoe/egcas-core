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

#include "egcunarynode.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"

EgcUnaryNode::EgcUnaryNode() : m_child(nullptr)
{

}

EgcUnaryNode::EgcUnaryNode(const EgcUnaryNode& orig) : EgcContainerNode(orig)
{
        EgcNode *originalChild = const_cast<EgcUnaryNode&>(orig).getChild(0);
        if (originalChild)
                m_child.reset(originalChild->copy());

        //set the parent also
        if(m_child)
                m_child->provideParent(this);
}

EgcUnaryNode::EgcUnaryNode(EgcUnaryNode&& orig) : EgcContainerNode(orig)
{
        EgcNode *originalChild = const_cast<EgcUnaryNode&>(orig).getChild(0);
        if (originalChild)
                m_child.reset(orig.takeOwnership(*originalChild));

        //set the parent also
        if(m_child)
                m_child->provideParent(this);
}

EgcUnaryNode::~EgcUnaryNode()
{
}

EgcUnaryNode& EgcUnaryNode::operator=(const EgcUnaryNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //and create a new one
        EgcNode *originalChild = rhs.getChild(0);
        if (originalChild)
                m_child.reset(originalChild->copy());

        return *this;
}

EgcUnaryNode& EgcUnaryNode::operator=(EgcUnaryNode&& rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //and create a new one
        EgcNode *originalChild = rhs.getChild(0);
        if (originalChild) {
                m_child.reset(rhs.takeOwnership(*originalChild));
        }

        return *this;
}

bool EgcUnaryNode::valid(void)
{
        if (m_child)
                if (m_child->valid())
                        return true;

        return false;
}

bool EgcUnaryNode::isUnaryNode(void) const
{
        return true;
}

void EgcUnaryNode::notifyContainerOnChildDeletion(EgcNode* child)
{
        if (m_child.data() == child)
                m_child.reset(nullptr);
}

void EgcUnaryNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        if (m_child.data() == &old_child) {
                (void) m_child.take();
                m_child.reset(&new_child);
        }
}

EgcNode* EgcUnaryNode::takeOwnership(EgcNode &child)
{
        EgcNode* retval = nullptr;

        if (m_child.data() == &child) {
                retval = m_child.take();
                retval->provideParent(nullptr);
        }

        return retval;
}

void EgcUnaryNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

EgcNode* EgcUnaryNode::getChild(quint32 index) const
{
        if (index == 0)
                return m_child.data();
        else
                return nullptr;
}

bool EgcUnaryNode::setChild(quint32 index, EgcNode& expression)
{
        bool retval = true;

        QScopedPointer<const EgcNode> expr(&expression);

        if (index == 0) {
                m_child.reset(const_cast<EgcNode*>(expr.take()));

                //set the parent also
                if(m_child)
                        m_child->provideParent(this);
        } else {
                retval = false;
        }

        return retval;
}

quint32 EgcUnaryNode::getNumberChildNodes(void) const
{
        return 1;
}

bool EgcUnaryNode::isFirstChild(EgcNode &child) const
{
        if (m_child.data() == &child)
                return true;
        else
                return false;
}

bool EgcUnaryNode::isLastChild(EgcNode &child) const
{
        if (m_child.data() == &child)
                return true;
        else
                return false;
}

EgcNode* EgcUnaryNode::incrementToNextChild(EgcNode &previousChild) const
{
        (void) previousChild;

        return nullptr;
}

EgcNode* EgcUnaryNode::decrementToPrevChild(EgcNode &previousChild) const
{
        (void) previousChild;

        return nullptr;
}

bool EgcUnaryNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        if (child.getParent() == this) {
                index = 0;
                return true;
        }

        return false;
}

bool EgcUnaryNode::operator==(const EgcNode& node) const
{
        bool retval = false;

        if (node.isUnaryNode() && this->getNodeType() == node.getNodeType()) {
                EgcNode* child = static_cast<const EgcUnaryNode&>(node).getChild(0);
                if (!m_child.isNull() && child) {
                        if (*m_child == *child)
                                retval = true;
                }
        }

        return retval;
}

