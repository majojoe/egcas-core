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

#include "egcscreenpos.h"
#include "specialNodes/egcnode.h"
#include <libegcas/eg_mml_document.h>

EgcScreenPos::EgcScreenPos()
{
        m_positions.clear();
}

void EgcScreenPos::setPositions(QVector<EgRenderingPosition> positions)
{
        m_positions.clear();
        EgRenderingPosition pos;
        quint64 id;
        foreach(pos, positions) {
                id = pos.m_nodeId | (static_cast<quint64>(pos.m_index) << 32);
                m_positions.insert(id, pos);
        }
}

EgRenderingPosition EgcScreenPos::getMathmlIdAtPos(const QPointF &pos)
{
        qreal w = 1.0e+37;      //choose values that are high enough
        qreal h = 1.0e+37;
        EgRenderingPosition i;
        EgRenderingPosition retval;

        foreach (i, m_positions) {
                if (i.m_itemRect.contains(pos))
                        if ( (i.m_itemRect.width() + i.m_itemRect.height()) < (w + h) ) {
                                w = i.m_itemRect.width();
                                h = i.m_itemRect.height();
                                retval = i;
                        }
        }

        return retval;
}

bool EgcScreenPos::empty(void)
{
        m_positions.empty();
}

EgRenderingPosition EgcScreenPos::findRenderingData(quint32 mathmId, quint32 subindex = 0)
{
        EgRenderingPosition retval;
        retval = m_positions.value(mathmId | (static_cast<quint64>(subindex) << 32));

        return retval;
}
