#include "egcsimpletextitem.h"
#include "egcasscene.h"

EgcSimpleTextItem::EgcSimpleTextItem(QGraphicsItem*parent) : QGraphicsSimpleTextItem(parent)
{
        init();
}

EgcSimpleTextItem::EgcSimpleTextItem(const QString & text, QGraphicsItem * parent) : QGraphicsSimpleTextItem(text, parent)
{
        init();
}

QPointF EgcSimpleTextItem::getPos( void ) const
{
        return pos();
}

void EgcSimpleTextItem::init()
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
}

QVariant EgcSimpleTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemPositionChange && scene()) {
         // value is the new position.
         QPointF newPos = value.toPointF();
         QSizeF grid = qobject_cast<EgCasScene*>(this->scene())->grid();
         newPos.setX(qRound(newPos.x()/grid.width()) * grid.width() );
         newPos.setY(qRound(newPos.y()/grid.height()) * grid.height() );

         return newPos;
     }
     return QGraphicsItem::itemChange(change, value);
 }
