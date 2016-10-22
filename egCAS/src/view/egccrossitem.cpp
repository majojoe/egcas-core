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

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QtCore/qmath.h>
#include "egccrossitem.h"

EgcCrossItem::EgcCrossItem(QGraphicsItem *parent) : m_line_hor{new QGraphicsLineItem(this)},
                                                    m_line_vert{new QGraphicsLineItem(this)}, m_pos{0.0, 0.0}
{
        setFlags(ItemIsMovable | ItemClipsToShape);
        m_line_hor->setPen(QPen(QBrush(QColor(Qt::green)), 2.0));
        m_line_vert->setPen(QPen(QBrush(QColor(Qt::green)), 2.0));
        m_line_hor->setLine(static_cast<qreal>(-m_size.width()/2), 0.0, static_cast<qreal>(m_size.width()/2), 0.0);
        m_line_vert->setLine(0.0, static_cast<qreal>(-m_size.height()/2), 0.0, static_cast<qreal>(m_size.height()/2));
        //hide();
}

QRectF EgcCrossItem::boundingRect() const
{
        QRectF bounds(static_cast<qreal>(-m_size.width()/2), static_cast<qreal>(-m_size.height()/2), m_size.width(),
                      m_size.height());
        return bounds;
}

void EgcCrossItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
        (void) option;
        (void) widget;

        painter->save();

        //painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(QPen(Qt::green));
        painter->setBrush(QBrush(Qt::green));
        QRectF boundRect(0.0, 0.0, m_handleSize.width(), m_handleSize.height());
        QVector<QPointF> triangle;
        triangle.append(boundRect.topRight());
        triangle.append(boundRect.bottomRight());
        triangle.append(boundRect.bottomLeft());
        painter->drawPolygon(QPolygonF(triangle));

        painter->restore();
}
