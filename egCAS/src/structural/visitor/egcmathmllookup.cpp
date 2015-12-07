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

#include <specialNodes/egcnode.h>
#include "egcmathmllookup.h"

EgcMathmlLookup::EgcMathmlLookup()
{

}

void EgcMathmlLookup::addId(EgcNode& node, quint32 id)
{
        m_lookup.insert(&node, id);
}

void EgcMathmlLookup::clear(void)
{
        m_lookup.clear();
}

quint32 EgcMathmlLookup::getIdFrame(EgcNode& node)
{
        quint32 retval = 0;

        QMultiHash<EgcNode*, quint32>::const_iterator i = m_lookup.find(&node);
        while (i != m_lookup.end() && i.key() == &node) {
                quint32 tmp = i.value();
                if (retval != 0)
                        retval = qMin(retval, tmp);
                else
                        retval = tmp;
                ++i;
        }

        return retval;
}

QList<quint32> EgcMathmlLookup::getIds(EgcNode& node)
{
        return m_lookup.values(&node);
}

QList<quint32> EgcMathmlLookup::getIdsNonFrame(EgcNode& node)
{
        quint32 id = 0;
        QList<quint32> list;

        QMultiHash<EgcNode*, quint32>::const_iterator i = m_lookup.find(&node);
        while (i != m_lookup.end() && i.key() == &node) {
                quint32 tmp = i.value();
                if (id != 0)
                        id = qMin(id, tmp);
                else
                        id = tmp;
                list.append(id);
                ++i;
        }

        if (!list.empty() && id != 0)
                list.removeAll(id);

        return list;
}

int EgcMathmlLookup::getIdCount(EgcNode& node)
{
        return m_lookup.values(&node).size();
}

EgcNode* EgcMathmlLookup::findNode(quint32 id)
{
#warning remove comment if code works
//        QHashIterator<EgcNode&, quint32> iter( m_lookup );
//        while( iter.hasNext() )
//        {
//                QList<quint32> list = m_lookup.values( iter.next().key() );
//                foreach( quint32 val, list )
//                {
//                        if (val == id)
//                                return &iter.key();
//                }
//        }

        QMultiHash<EgcNode*, quint32>::const_iterator i = m_lookup.begin();
        while (i != m_lookup.end()) {
            if (i.value() == id)
                    return i.key();
            ++i;
        }

        return nullptr;
}
