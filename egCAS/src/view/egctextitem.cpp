#include <QTextCursor>
#include "egctextitem.h"
#include "egcasscene.h"

EgcTextItem::EgcTextItem(QGraphicsItem*parent) : QGraphicsTextItem(parent)
{
        init();
}

EgcTextItem::EgcTextItem(const QString & text, QGraphicsItem * parent) : QGraphicsTextItem(text, parent)
{
        init();
}

QPointF EgcTextItem::getPos( void ) const
{
        return pos();
}

void EgcTextItem::init()
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
}

QVariant EgcTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
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

void EgcTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event)
{
        setTextInteractionFlags(Qt::TextEditable | Qt::TextEditorInteraction);
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
        setFocus();
        QGraphicsItem::mouseDoubleClickEvent(event);
}

void EgcTextItem::focusOutEvent(QFocusEvent *event)
{
        QTextCursor cursor(textCursor());
        cursor.clearSelection();
        setTextCursor(cursor);
        setTextInteractionFlags(Qt::NoTextInteraction);
        QGraphicsItem::focusOutEvent(event);
}
