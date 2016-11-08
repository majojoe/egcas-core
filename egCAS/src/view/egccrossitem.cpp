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
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include "egccrossitem.h"
#include "egcasscene.h"

EgcCrossItem::EgcCrossItem(QGraphicsItem *parent)
{
        setFlags( ItemSendsScenePositionChanges | ItemClipsChildrenToShape | ItemClipsToShape | ItemIsFocusable);
        hide();
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
        painter->setPen(QPen(Qt::darkGreen));
        painter->setBrush(QBrush(Qt::darkGreen));
        painter->drawLine(static_cast<qreal>(-m_size.width()/2), 0, static_cast<qreal>(m_size.width()/2), 0);
        painter->drawLine(0, static_cast<qreal>(-m_size.height()/2), 0, static_cast<qreal>(m_size.height()/2));
        painter->restore();
}

QVariant EgcCrossItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
     if (change == ItemPositionChange && scene()) {
         // value is the new position.
         QPointF newPos = snapGrid(value.toPointF());

         return newPos;
     }
     return QGraphicsItem::itemChange(change, value);
}

void EgcCrossItem::keyPressEvent(QKeyEvent *keyEvent)
{
        QGraphicsScene* scn = scene();
        if (!scn)
                return;

        int key = keyEvent->key();
        if (    key == Qt::Key_Left
             || key == Qt::Key_Right
             || key == Qt::Key_Up
             || key == Qt::Key_Down) {
                keyEvent->accept();

                switch(keyEvent->key()) {
                case Qt::Key_Up:
                        if (scenePos().y() > 0)
                                up();
                        break;
                case Qt::Key_Down:
                        if (scenePos().y() < scn->sceneRect().height() - getGrid().height())
                                down();
                        break;
                case Qt::Key_Left:
                        if (scenePos().x() > 0)
                                left();
                        break;
                default:  //right
                        if (scenePos().x() < scn->sceneRect().width() - getGrid().width())
                                right();
                        break;
                }

                QGraphicsItem* item = selectedItem();
                if (item) {
                        item->setSelected(true);
                        item->setFocus();
                }
                ensureVisible(boundingRect(), 0, 0);
        } else {
                QGraphicsItem::keyPressEvent(keyEvent);
                if (!selectedItem()) {
                        EgCasScene *scene = qobject_cast<EgCasScene*>(scn);
                        if (scene) {
                                scene->triggerFormulaCreation(scenePos(), keyEvent);
                        }
                }
        }
}

QGraphicsItem *EgcCrossItem::selectedItem(void) const
{
        QGraphicsScene* scn = scene();
        if (!scn)
                return nullptr;

        QGraphicsItem* item = scn->itemAt(scenePos(), QTransform());
        if (item) {
                if (item != this)
                        return item;
        }

        return nullptr;
}

void EgcCrossItem::focusOutEvent(QFocusEvent * event)
{
        hide();
        QGraphicsItem::focusOutEvent(event);
}

void EgcCrossItem::focusInEvent(QFocusEvent * event)
{
        show();
        QGraphicsItem::focusInEvent(event);
}

QPointF EgcCrossItem::snapGrid(const QPointF& pos)
{
        // value is the new position.
        QPointF newPos = pos;
        QSizeF grid = getGrid();
        if (grid.isValid()) {
                newPos.setX(qRound(newPos.x()/grid.width()) * grid.width() );
                newPos.setY(qRound(newPos.y()/grid.height()) * grid.height() );
        }

        return newPos;
}

QSizeF EgcCrossItem::getGrid(void)
{
        QSizeF grid;

        QGraphicsScene *scene = this->scene();
        if (scene) {
                grid = static_cast<EgCasScene*>(scene)->grid();
        }

        return grid;
}

void EgcCrossItem::up(void)
{
        moveBy(0, -getGrid().height());
        show();
}

void EgcCrossItem::down(void)
{
        moveBy(0, getGrid().height());
        show();
}

void EgcCrossItem::left(void)
{
        moveBy(-getGrid().width(), 0);
        show();
}

void EgcCrossItem::right(void)
{
        moveBy(getGrid().width(), 0);
        show();
}

void EgcCrossItem::up(qreal yCoord)
{
        QPointF point = snapGrid(QPointF(pos().x(), yCoord));
        setPos(point);
        show();
        if (point.y() >= yCoord)
                up();
}

void EgcCrossItem::down(qreal yCoord)
{
        QPointF point = snapGrid(QPointF(pos().x(), yCoord));
        setPos(point);
        show();
        if (point.y() <= yCoord)
                down();
}

void EgcCrossItem::left(qreal xCoord)
{
        QPointF point = snapGrid(QPointF(xCoord, pos().y()));
        setPos(point);
        show();
        if (point.x() >= xCoord)
                left();
}

void EgcCrossItem::right(qreal xCoord)
{
        QPointF point = snapGrid(QPointF(xCoord, pos().y()));
        setPos(point);
        show();
        if (point.x() <= xCoord)
                right();
}
