#include "egcpixmapitem.h"
#include "egcasscene.h"
#include "resizehandle.h"

EgcPixmapItem::EgcPixmapItem(QGraphicsItem*parent) : QGraphicsPixmapItem(parent)
{
        init();
}

EgcPixmapItem::EgcPixmapItem(const QPixmap&pixmap, QGraphicsItem * parent) : QGraphicsPixmapItem(pixmap, parent)
{
        init();
}

EgcPixmapItem::~EgcPixmapItem()
{        
}

QPointF EgcPixmapItem::getPos( void ) const
{
        return pos();
}

void EgcPixmapItem::init()
{
        m_resizeHandle.reset(new ResizeHandle(this, QSizeF(8.0, 8.0)));
        setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
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
