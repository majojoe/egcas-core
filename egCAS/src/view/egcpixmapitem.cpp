#include "egcpixmapitem.h"
#include "egcasscene.h"
#include "resizehandle.h"

EgcPixmapItem::EgcPixmapItem(QGraphicsItem*parent) : QGraphicsPixmapItem(parent), m_resizeHandle(NULL), m_childSelectionState(false)
{
        init();
}

EgcPixmapItem::EgcPixmapItem(const QPixmap&pixmap, QGraphicsItem * parent) : QGraphicsPixmapItem(pixmap, parent), m_resizeHandle(NULL), m_childSelectionState(false)
{
        init();
}

QPointF EgcPixmapItem::getPos( void ) const
{
        return pos();
}

void EgcPixmapItem::init()
{
        setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
        m_resizeHandle = new ResizeHandle(QSizeF(8.0, 8.0), this);
        m_resizeHandle->setPos(boundingRect().bottomRight());
        m_resizeHandle->hide();
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

        if (change == ItemSelectedHasChanged) {
                if (value.toBool()) {
                        m_resizeHandle->show();
                        m_childSelectionState = true;
                } else {
                        m_resizeHandle->hide();
                        m_childSelectionState = false;
                }
        }

        return QGraphicsItem::itemChange(change, value);
 }

void EgcPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
        QGraphicsItem::mouseReleaseEvent(event);
        if (m_childSelectionState)
                m_resizeHandle->setSelected(true);
        else
                m_resizeHandle->setSelected(false);
}
