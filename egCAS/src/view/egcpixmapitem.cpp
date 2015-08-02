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

#include "egcpixmapitem.h"
#include "egcasscene.h"
#include "resizehandle.h"

EgcPixmapItem::EgcPixmapItem(QGraphicsItem*parent) : QGraphicsPixmapItem{parent}, m_entity{nullptr}
{
        m_resizeHandle.reset(new ResizeHandle(this, QSizeF(8.0, 8.0)));
        setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
}

EgcPixmapItem::EgcPixmapItem(const QPixmap&pixmap, QGraphicsItem * parent) : EgcPixmapItem{parent}
{
        QGraphicsPixmapItem::setPixmap(pixmap);
}

EgcPixmapItem::EgcPixmapItem(const QPointF point, QGraphicsItem *parent) : EgcPixmapItem{parent}
{
        QGraphicsPixmapItem::setPos(point);
}

EgcPixmapItem::~EgcPixmapItem()
{        
}

QVariant EgcPixmapItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
        if (change == ItemPositionChange && scene()) {
                // value is the new position.
                QPointF newPos = value.toPointF();
                QSizeF grid = qobject_cast<EgCasScene*>(this->scene())->grid();
                newPos.setX(qRound(newPos.x()/grid.width()) * grid.width() );
                newPos.setY(qRound(newPos.y()/grid.height()) * grid.height() );

                return newPos;
        }

        m_resizeHandle->itemChangeInfo(change, value, scene());

        return QGraphicsItem::itemChange(change, value);
 }

void EgcPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
        QGraphicsItem::mouseReleaseEvent(event);

        m_resizeHandle->mouseReleaseEventInfo();
}

void EgcPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent*event)
{
        QGraphicsItem::mouseMoveEvent(event);

        m_resizeHandle->mouseMoveEventInfo();
}

void EgcPixmapItem::setEntity(EgcAbstractPixmapEntity* entity)
{
        m_entity = entity;
}

QPointF EgcPixmapItem::getPosition( void ) const
{
        return pos();
}

void EgcPixmapItem::setPosition(const QPointF &point)
{
        setPos(point);
}

void EgcPixmapItem::setScaleFactor(qreal scaleFactor)
{
        setScale(scaleFactor);
}

QSize EgcPixmapItem::getSize(void)
{
        boundingRect().size();
}

void EgcPixmapItem::setPixmap(QPixmap pixmap)
{
        QGraphicsPixmapItem::setPixmap(pixmap);
}

QPixmap EgcPixmapItem::getPixmap(void)
{
        return pixmap();
}
