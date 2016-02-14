/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
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

#include "egcopprec.h"
#include "structural/specialNodes/egcnode_gen.h"
#include "structural/specialNodes/egcnode.h"

bool EgcOpPrec::s_alreadyInitialized = false;
QHash<int, quint32> EgcOpPrec::s_bindingPower;

EgcOpPrec::EgcOpPrec(EgcNode& node) : m_type{EgcNodeType::NodeUndefined}
{
        if (!s_alreadyInitialized) {
                s_alreadyInitialized = true;

                s_bindingPower.insert(static_cast<int>(EgcNodeType::ParenthesisNode), 70);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::RootNode), 60);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::ExponentNode), 60);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::UnaryMinusNode), 50);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::MultiplicationNode), 30);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::DivisionNode), 30);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::MinusNode), 20);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::PlusNode), 20);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::DefinitionNode), 2);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::EqualNode), 1);
                // the following have no binding power, since these operations are always top level (breakout not possible)
                s_bindingPower.insert(static_cast<int>(EgcNodeType::DifferentialNode), 0);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::FunctionNode), 0);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::IntegralNode), 0);
                s_bindingPower.insert(static_cast<int>(EgcNodeType::BaseNode), 0);
        }

        if (node.isOperation()) {
                m_type = node.getNodeType();
        }

}

bool EgcOpPrec::operator<(const EgcOpPrec& rhs) const
{
        EgcNodeType type1 = getType();
        EgcNodeType type2 = rhs.getType();
        quint32 bp1 = 0;
        quint32 bp2 = 0;

        if (s_bindingPower.contains(static_cast<int>(type1)))
                bp1 = s_bindingPower.value(static_cast<int>(type1));

        if (s_bindingPower.contains(static_cast<int>(type2)))
                bp2 = s_bindingPower.value(static_cast<int>(type2));

        if (bp1 < bp2)
                return true;
        else
                return false;
}

bool EgcOpPrec::operator>(const EgcOpPrec& rhs) const
{
        if (*this == rhs)
                return false;

        if (*this < rhs)
                return false;

        return true;
}

bool EgcOpPrec::operator==(const EgcOpPrec& opPrec) const
{
        bool retval = false;

        if (opPrec.getType() == m_type)
                retval = true;

        return retval;
}

EgcNodeType EgcOpPrec::getType(void) const
{
        return m_type;
}
