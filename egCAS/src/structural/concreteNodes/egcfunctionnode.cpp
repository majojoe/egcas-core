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
        QScopedPointer<EgcEmptyNode> val(new EgcEmptyNode());
        if (val) {
                setChild(0, *val.take());
        }
}

bool EgcFunctionNode::transferArgs(EgcArgumentsNode& args)
{
        EgcFlexNode& node = *this;
        node = std::move(args);

        return true;
}

void EgcFunctionNode::setName(const QString& fncName)
{
        m_fncName = fncName;
}

QString EgcFunctionNode::getName(void)
{
        return m_fncName;
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

        return false;
}

bool EgcFunctionNode::valid(void)
{
        if (m_fncName.isEmpty())
                return false;
        else
                return true;
}

void EgcFunctionNode::setStuffedName(const QString& fncName)
{
        m_fncName = EgcAlnumNode::decode(fncName);
}

QString EgcFunctionNode::getStuffedName()
{
        return EgcAlnumNode::encode(m_fncName);
}
