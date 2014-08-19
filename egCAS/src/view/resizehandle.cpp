#include <QPainter>
#include <QGraphicsItem>
#include <QCursor>
#include "resizehandle.h"

ResizeHandle::ResizeHandle(const QSizeF& size, QGraphicsItem *parent) :
        QGraphicsItem(parent), m_handleSize(size)
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable);
}

QRectF ResizeHandle::boundingRect() const
{
        qreal marginX = m_handleSize.width() / 2.0;
        qreal marginY = m_handleSize.height() / 2.0;
        QRectF bounds(0.0, 0.0, m_handleSize.width(), m_handleSize.height());
        bounds.adjust(-marginX, -marginY, marginX, marginY);
        return bounds;
}

//void ResizeHandle::mousePressEvent(QGraphicsSceneMouseEvent * event)
//{

//}

//void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
//{

//}

//void ResizeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
//{

//}


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

         if (selected)
                 setCursor(QCursor(Qt::SizeFDiagCursor));
         else
                 setCursor(QCursor(Qt::ArrowCursor));
     }

     return QGraphicsItem::itemChange(change, value);
 }
