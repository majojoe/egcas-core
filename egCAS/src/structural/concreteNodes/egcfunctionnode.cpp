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

#include "egcfunctionnode.h"
#include "egcalnumnode.h"
#include "structural/specialNodes/egcemptynode.h"


EgcFunctionNode::EgcFunctionNode()
{
        QScopedPointer<EgcEmptyNode> name(new EgcEmptyNode());
        setChild(0, *name.take());
        QScopedPointer<EgcEmptyNode> val(new EgcEmptyNode());
        if (val) {
                if (insert(1, *val.data()))
                        (void) val.take();
        }
}

bool EgcFunctionNode::transferArgs(EgcArgumentsNode& args)
{
        EgcFlexNode& node = *this;
        node = std::move(args);
        QScopedPointer<EgcEmptyNode> val(new EgcEmptyNode());
        if (val) {
                if (insert(0, *val.data()))
                        (void) val.take();
        }
}

void EgcFunctionNode::setName(const QString& fncName)
{
        if (m_childs.size() == 0)
                return;
        if (!m_childs.at(0))
                return;

        EgcAlnumNode *alnum;
        if (m_childs.at(0)->getNodeType() == EgcNodeType::EmptyNode) {
                QScopedPointer<EgcAlnumNode> val(new EgcAlnumNode());
                if (setChild(0, *val.data()))
                        (void) val.take();
        }

        if (m_childs.at(0)->getNodeType() == EgcNodeType::AlnumNode) {
                alnum = static_cast<EgcAlnumNode*>(m_childs.at(0));
                alnum->setValue(fncName);
        }
}

QString EgcFunctionNode::getName(void)
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

bool EgcFunctionNode::cursorSnaps(EgcNodeSide side) const
{
        if (side == EgcNodeSide::right)
                return true;

        return false;
}

bool EgcFunctionNode::visibleSigns(EgcNodeSide side) const
{
        (void) side;

        return true;
}

bool EgcFunctionNode::determineIfChildIsAtomicallyBound(const EgcNode* node) const
{
        quint32 ind = 0;

        if (!node)
                return false;

        (void) getIndexOfChild(*const_cast<EgcNode*>(node), ind);

        if (ind == 0)
                return true;

        return false;
}

bool EgcFunctionNode::valid(void)
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
