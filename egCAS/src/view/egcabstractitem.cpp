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

#include <QSizeF>
#include <QPointF>
#include <QtMath>
#include "egcabstractitem.h"
#include "egcasscene.h"

EgcAbstractItem::EgcAbstractItem() : m_gridActivated{true}
{
        
}

QPointF EgcAbstractItem::snap(const QPointF& pos)
{
        // value is the new position.
        QPointF newPos = pos;
        EgCasScene* scn = getEgcScene();
        if (scn) {
                const EgcWorksheet& sheet = scn->worksheet();
                QSizeF grid = scn->grid();
                QRectF iRect = bRect();
                iRect.moveTopLeft(newPos);
                newPos = sheet.snapWorksheet(iRect);
                QPointF tmpPos = newPos;
                if (grid.isValid() && m_gridActivated) {
                        newPos.setX(qFloor(tmpPos.x()/grid.width()) * grid.width() );
                        if (newPos.x() < sheet.getLeftMargin())
                                newPos.setX(qCeil(tmpPos.x()/grid.width()) * grid.width() );

                        tmpPos = newPos;
                        newPos.setY(qFloor(tmpPos.y()/grid.height()) * grid.height() );
                        if (!sheet.isVisible(newPos))
                                newPos.setY(qCeil(tmpPos.y()/grid.height()) * grid.height() );
                }
        }

        return newPos;
}

void EgcAbstractItem::activateGrid(bool activate)
{
        m_gridActivated = activate;
}

bool EgcAbstractItem::isGridActivated(void)
{
        return m_gridActivated;
}
