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

#include <QRegularExpression>
#include "egcnumbernode.h"


QRegularExpression EgcNumberNode::s_validator = QRegularExpression("[-+.0-9]+");
bool EgcNumberNode::s_regexInitialized = false;


EgcNumberNode::EgcNumberNode() : m_value("0.0")
{
        if (!s_regexInitialized) {
                s_regexInitialized = true;
                s_validator.optimize();
        }
}

EgcNumberNode::~EgcNumberNode()
{

}

void EgcNumberNode::setValue(const QString& value)
{
        m_value = value;
}

QString EgcNumberNode::getValue(void) const
{
        return m_value;
}

bool EgcNumberNode::operator==(const EgcNode& node) const
{
        bool retval = false;

        if (node.getNodeType() == EgcNodeType::NumberNode) {
                if (static_cast<const EgcNumberNode&>(node).getValue() == m_value)
                        retval = true;
        }

        return retval;
}

int EgcNumberNode::nrSubindexes(void) const
{
        return m_value.size();
}

bool EgcNumberNode::insert(QChar character, int position)
{
        bool retval = false;

        if (s_validator.match(character).hasMatch()) {
                if (position <= m_value.size() && position >= 0) {
                        m_value.insert(position, character);
                        retval = true;
                }
        }

        return retval;
}

bool EgcNumberNode::remove(int position)
{
        bool retval = false;

        if (position >= 0 && position < m_value.size()) {
                retval = true;
                m_value.remove(position, 1);
        }

        return retval;
}

