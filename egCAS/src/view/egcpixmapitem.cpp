#include "egcpixmapitem.h"
#include "egcasscene.h"
#include "resizehandle.h"

EgcPixmapItem::EgcPixmapItem(QGraphicsItem*parent) : QGraphicsPixmapItem(parent), m_resizeHandle(NULL), m_childSelectionState(false),
                                                     m_resizeHandleAdded(false)
{
        init();
}

EgcPixmapItem::EgcPixmapItem(const QPixmap&pixmap, QGraphicsItem * parent) : QGraphicsPixmapItem(pixmap, parent), m_resizeHandle(NULL), m_childSelectionState(false),
                                                                             m_resizeHandleAdded(false)
{
        init();
}

EgcPixmapItem::~EgcPixmapItem()
{
        scene()->removeItem(m_resizeHandle);
        delete(m_resizeHandle);
}

QPointF EgcPixmapItem::getPos( void ) const
{
        return pos();
}

void EgcPixmapItem::init()
{
        setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
        m_resizeHandle = new ResizeHandle(this, QSizeF(8.0, 8.0));
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
                        if (!m_resizeHandleAdded && scene()) {
                                scene()->addItem(m_resizeHandle);
                                m_resizeHandle->setPos(mapToScene(boundingRect().bottomRight()));
                                m_resizeHandleAdded = true;
                        }

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

void EgcPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent*event)
{
        QGraphicsItem::mouseMoveEvent(event);

        m_resizeHandle->setPos(mapToScene(boundingRect().bottomRight()));
}
