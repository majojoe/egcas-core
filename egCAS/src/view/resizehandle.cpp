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
#include "resizehandle.h"

ResizeHandle::ResizeHandle(QGraphicsItem *content, const QSizeF& size) :
        m_resizableContent(content), m_handleSize(size), m_contentStartScale(0.0), m_contentStartDiag(0.0), m_addedToScene(false), m_selectionState(false)
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable);
        hide();
}

QRectF ResizeHandle::boundingRect() const
{
        qreal marginX = m_handleSize.width() / 2.0;
        qreal marginY = m_handleSize.height() / 2.0;
        QRectF bounds(0.0, 0.0, m_handleSize.width(), m_handleSize.height());
        bounds.adjust(-marginX, -marginY, marginX, marginY);
        return bounds;
}

void ResizeHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
        m_scaleStartPos = mapToScene(event->pos());
        m_resizableContent->setFlag(ItemIsMovable, false);
        QGraphicsItem::mousePressEvent(event);
        m_contentStartScale = m_resizableContent->scale();

        qreal content_width = m_resizableContent->boundingRect().width();
        qreal content_height = m_resizableContent->boundingRect().height();


        m_contentStartDiag = sqrt(qPow(content_width, 2) + qPow(content_height, 2));
}

void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{

        QGraphicsItem::mouseMoveEvent(event);

        QPointF event_pos = mapToScene(event->pos());
        qreal width = event_pos.x() - m_scaleStartPos.x();
        qreal height = event_pos.y() - m_scaleStartPos.y();
        qreal diag = sqrt(qPow(width, 2) + qPow(height, 2));
        if (width < 0.0)
                diag = -1.0 * diag;

        qreal m_scale = (diag/m_contentStartDiag) + m_contentStartScale;

        m_resizableContent->setScale(m_scale);
}

void ResizeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{        
        QGraphicsItem::mouseReleaseEvent(event);
        m_resizableContent->setFlag(ItemIsMovable, true);
        setPos(mapToScene(mapFromItem(m_resizableContent, m_resizableContent->boundingRect().bottomRight())));
}

void ResizeHandle::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
        (void) option;
        (void) widget;

        painter->save();

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(QPen(Qt::blue, m_handleSize.width() * 0.1, Qt::SolidLine));
        painter->setBrush(QBrush(Qt::blue));
        QRectF boundRect(0.0, 0.0, m_handleSize.width(), m_handleSize.height());
        QVector<QPointF> triangle;
        triangle.append(boundRect.topRight());
        triangle.append(boundRect.bottomRight());
        triangle.append(boundRect.bottomLeft());
        painter->drawPolygon(QPolygonF(triangle));

        painter->restore();
}

QVariant ResizeHandle::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemSelectedChange) {
         // value is the new position.
         bool selected = value.toBool();

         if (selected) {
                 setCursor(QCursor(Qt::SizeFDiagCursor));
         } else {
                 setCursor(QCursor(Qt::ArrowCursor));
         }
     }

     return QGraphicsItem::itemChange(change, value);
 }

void ResizeHandle::itemChangeInfo(GraphicsItemChange change, const QVariant &value, QGraphicsScene *content_scene)
{
        if (change == ItemSelectedHasChanged) {
                if (value.toBool()) {
                        show();
                        m_selectionState = true;
                        if (!m_addedToScene && content_scene) {
                                content_scene->addItem(this);
                                setPos(mapToScene(mapFromItem(m_resizableContent, m_resizableContent->boundingRect().bottomRight())));
                                m_addedToScene = true;
                        }

                } else {
                        hide();
                        m_selectionState = false;
                }
        }

        //remove the old scene
        if (change == ItemSceneChange) {
                QGraphicsScene* old_scene = (QGraphicsScene*)value.value<void*>();
                if (old_scene) {
                        old_scene->removeItem(this);
                        m_addedToScene = false;
                }
        }
}

void ResizeHandle::mouseReleaseEventInfo(void)
{
        if (m_selectionState)
                setSelected(true);
        else
                setSelected(false);
}

void ResizeHandle::mouseMoveEventInfo(void)
{
        setPos(mapToScene(mapFromItem(m_resizableContent, m_resizableContent->boundingRect().bottomRight())));
}
