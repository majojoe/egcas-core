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

#include "egccontainernode.h"
#include "../visitor/egcnodevisitor.h"

EgcNode::EgcNode() : m_parent(nullptr)
{
}

EgcNode::~EgcNode()
{
        notifyContainerOnChildDeletion(this);
}

bool EgcNode::valid(void)
{
        return true;
}

bool EgcNode::isContainer(void) const
{
        return false;
}

bool EgcNode::isUnaryNode(void) const
{
        return false;
}

bool EgcNode::isBinaryNode(void) const
{
        return false;
}

EgcContainerNode *EgcNode::getParent(void) const
{
        return m_parent;
}

void EgcNode::provideParent(EgcContainerNode* parent)
{
        m_parent = parent;
}

void EgcNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

bool EgcNode::isChild(EgcNode& parent)
{
        if (&parent == m_parent)
                return true;
        else
                return false;
}

bool EgcNode::isFlexNode(void) const
{
        return false;
}

bool EgcNode::operator==(const EgcNode& node) const
{
        (void) node;
        return false;
}

int EgcNode::nrSubindexes(void) const
{
        return 0;
}

bool EgcNode::isOperation(void) const
{
        return false;
}

bool EgcNode::cursorSnaps(EgcNodeSide side) const
{
        (void) side;
        
        return false;
}

bool EgcNode::visibleSigns(EgcNodeSide side) const
{
        (void) side;
        
        return false;        
}

bool EgcNode::modifyableElement(EgcNodeSide side) const
{
        return this->visibleSigns(side);
}
