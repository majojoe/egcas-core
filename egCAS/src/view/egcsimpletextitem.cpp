#include "egcsimpletextitem.h"

EgcSimpleTextItem::EgcSimpleTextItem(QGraphicsSimpleTextItem *parent) : QGraphicsSimpleTextItem(parent)
{
}

EgcSimpleTextItem::EgcSimpleTextItem(const QString & text, QGraphicsSimpleTextItem * parent) : QGraphicsSimpleTextItem(text, parent)
{

}

QPointF EgcSimpleTextItem::getPos( void ) const
{
        return pos();
}
