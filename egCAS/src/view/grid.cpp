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
#include "grid.h"


using namespace egcas;

Grid::Grid(EgCasScene& scene) : m_scene{scene}, m_activated{true}
{
}

Grid::Grid(EgCasScene& scene, QSize size) : m_scene{scene}, m_grid{size}, m_activated{true}
{
}

void Grid::setGrid(QSize size)
{
        m_grid = size;
}

QSize Grid::grid(void) const
{
        return m_grid;
}

void Grid::activate(bool activate)
{
        m_activated = activate;
}

bool Grid::isActivated(void) const
{
        return m_activated;
}

QPointF Grid::snap(const QPointF& point) const
{
        QPointF newPos = pos;
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

        return newPos;
}
