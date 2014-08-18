#include <QPainter>
#include <QGraphicsItem>
#include "resizehandle.h"

ResizeHandle::ResizeHandle(const QSizeF& size, QGraphicsItem *parent) :
        QGraphicsItem(parent), m_handleSize(size)
{
        setFlags(ItemIsMovable | ItemClipsToShape);
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
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(QPen(Qt::blue, m_handleSize.width() * 0.1, Qt::SolidLine));
        QRectF boundRect(0.0, 0.0, m_handleSize.width(), m_handleSize.height());
        painter->drawLine(boundRect.topRight(), boundRect.bottomRight());
        painter->drawLine(boundRect.bottomRight(), boundRect.bottomLeft());
        painter->drawLine(boundRect.topRight(), boundRect.bottomLeft());

        painter->restore();
}
