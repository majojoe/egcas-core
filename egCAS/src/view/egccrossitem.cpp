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
#include <QtMath>
#include "egccrossitem.h"
#include "egcasscene.h"
#include "actions/egcactionmapper.h"

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
         QPointF newPos = snapCursor(value.toPointF());

         m_lastPosition = newPos;

         return newPos;
     }
     return QGraphicsItem::itemChange(change, value);
}

void EgcCrossItem::keyPressEvent(QKeyEvent *keyEvent)
{
        bool isSignOrOperation = false;
        QGraphicsScene* scn = scene();
        if (!scn)
                return;
        EgCasScene *escn = qobject_cast<EgCasScene*>(scn);
        if (!escn)
                return;

        keyEvent->accept();

        switch(keyEvent->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
                escn->moveItems(true, scenePos());
                if (scenePos().y() < scn->sceneRect().height() - grid().height())
                        down();
                break;
        case Qt::Key_Backspace:
                escn->moveItems(false, scenePos());
                if (scenePos().y() > 0)
                        up();
                break;
        case Qt::Key_Delete:
                escn->moveItems(false, scenePos());
                break;
        case Qt::Key_Up:
                if (scenePos().y() > 0)
                        up();
                break;
        case Qt::Key_Down:
                if (scenePos().y() < scn->sceneRect().height() - grid().height())
                        down();
                break;
        case Qt::Key_Left:
                if (scenePos().x() > 0)
                        left();
                break;
        case Qt::Key_Right:
                if (scenePos().x() < scn->sceneRect().width() - grid().width())
                        right();
                break;
        case Qt::Key_Home:
                if (keyEvent->modifiers() == Qt::KeyboardModifier::ControlModifier)
                        documentBegin();
                else
                        lineBegin();
                break;
        case Qt::Key_End:
                if (keyEvent->modifiers() == Qt::KeyboardModifier::ControlModifier)
                        documentEnd();
                else
                        lineEnd();
                break;
        default:
                isSignOrOperation = true;
                QGraphicsItem::keyPressEvent(keyEvent);
                EgcOperations op = EgcActionMapper::map(keyEvent).m_op;
                if (    !selectedItem()
                     && (op == EgcOperations::mathCharOperator || op == EgcOperations::alnumKeyPressed)) {
                        escn->triggerFormulaCreation(scenePos(), EgcActionMapper::map(keyEvent));
                }
                break;
        }

        if (!isSignOrOperation) {
                QGraphicsItem* item = selectedItem();
                if (item) {
                        item->setSelected(true);
                        item->setFocus();
                }
                ensureVisible(boundingRect(), 0, 0);
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

QPointF EgcCrossItem::snapCursor(const QPointF& pos) const
{
        QPointF newPos = pos;
        QGraphicsScene *scene = this->scene();
        EgCasScene* scn = nullptr;
        if (scene) {
                scn = static_cast<EgCasScene*>(scene);
        }
        if (scn) {
                const EgcWorksheet& sheet = scn->worksheet();
                QSizeF grid = scn->grid().grid();
                newPos = sheet.snapWorksheet(newPos);
                QPointF tmpPos = newPos;
                if (grid.isValid()) {
                        newPos.setX(qRound(tmpPos.x()/grid.width()) * grid.width() );
                        if (newPos.x() < sheet.getLeftMargin())
                                newPos.setX(qCeil(tmpPos.x()/grid.width()) * grid.width() );
                        if (newPos.x() > (sheet.getSize().width() - sheet.getRightMargin()))
                                newPos.setX(qFloor(tmpPos.x()/grid.width()) * grid.width() );

                        tmpPos = newPos;
                        newPos.setY(qRound(tmpPos.y()/grid.height()) * grid.height() );
                        if (!sheet.isVisible(newPos))
                                newPos.setY(qCeil(tmpPos.y()/grid.height()) * grid.height() );
                        if (!sheet.isVisible(newPos))
                                newPos.setY(qFloor(tmpPos.y()/grid.height()) * grid.height() );
                }
        }

        return newPos;
}

QSizeF EgcCrossItem::grid(void)
{
        QSizeF grid;

        QGraphicsScene *scene = this->scene();
        if (scene) {
                grid = static_cast<EgCasScene*>(scene)->grid().grid();
        }

        return grid;
}

void EgcCrossItem::up(void)
{
        QGraphicsScene *scene = this->scene();
        if (scene) {
                static_cast<EgCasScene*>(scene)->moveUp(this, true);
                ensureVisible(QRectF(), 100.0, 100.0);
        }
        show();
}

void EgcCrossItem::down(void)
{
        QGraphicsScene *scene = this->scene();
        if (scene) {
                static_cast<EgCasScene*>(scene)->moveDown(this, true);
                ensureVisible(QRectF(), 100.0, 100.0);
        }
        show();
}

void EgcCrossItem::left(void)
{
        moveBy(-grid().width(), 0);
        ensureVisible();
        show();
}

void EgcCrossItem::right(void)
{
        moveBy(grid().width(), 0);
        ensureVisible();
        show();
}

void EgcCrossItem::lineBegin(void)
{
        setPos(0.0, pos().y());
        ensureVisible();
        show();
}

void EgcCrossItem::lineEnd(void)
{
        QGraphicsScene *scene = this->scene();
        if (scene) {
                qreal lineEndPos = static_cast<EgCasScene*>(scene)->worksheet().getSize().width();
                setPos(lineEndPos, pos().y());
        }
        ensureVisible();
        show();
}

void EgcCrossItem::documentBegin(void)
{
        setPos(pos().x(), 0.0);
        ensureVisible();
        show();
}

void EgcCrossItem::documentEnd(void)
{
        QGraphicsScene *scene = this->scene();
        if (scene) {
                qreal bottomMargin = static_cast<EgCasScene*>(scene)->worksheet().getBottomMargin();
                setPos(pos().x(), scene->sceneRect().height() - bottomMargin);
        }
        ensureVisible();
        show();
}

void EgcCrossItem::up(qreal yCoord)
{
        QPointF point = snapCursor(QPointF(pos().x(), yCoord));
        setPos(point);
        show();
        if (point.y() >= yCoord)
                up();
}

void EgcCrossItem::down(qreal yCoord)
{
        QPointF point = snapCursor(QPointF(pos().x(), yCoord));
        setPos(point);
        show();
        if (point.y() <= yCoord)
                down();
}

void EgcCrossItem::left(qreal xCoord)
{
        QPointF point = snapCursor(QPointF(xCoord, pos().y()));
        setPos(point);
        show();
        if (point.x() >= xCoord)
                left();
}

void EgcCrossItem::right(qreal xCoord)
{
        QPointF point = snapCursor(QPointF(xCoord, pos().y()));
        setPos(point);
        show();
        if (point.x() <= xCoord)
                right();
}

QPointF EgcCrossItem::getLastPosition(void) const
{
        return snapCursor(m_lastPosition);
}
