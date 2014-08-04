#ifndef EGCTEXTITEM_H
#define EGCTEXTITEM_H

#include <QGraphicsSimpleTextItem>
#include "egcasiteminterface.h"

class EgcTextItem: public QGraphicsTextItem
{
public:
        ///std constructor
        explicit EgcTextItem(QGraphicsItem *parent = 0);
        ///constructor for text message
        EgcTextItem(const QString & text, QGraphicsItem * parent = 0);
        ///std destructor
        virtual ~EgcTextItem() { }
        /**
         * @brief getPos overloads the inherited get function for the position of the item
         * @return the position of the item
         */
        virtual QPointF getPos( void ) const;
protected:
        /**
         * @brief init initializes a new instance of this class (used in constructor)
         */
        void init(void);
        /**
         * @brief itemChange reimplements change function of QGraphicsItem to be able to realize a grid
         * @param change enum that describes state changes that are notified
         * @param value the value that has changed
         * @return the value that has been adjusted
         */
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        /**
         * @brief mouseDoubleClickEvent overrides mousePressEvent from QGraphicsItem
         * @param event pointer to QGraphicsSceneMouseEvent
         */
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
        /**
         * @brief focusOutEvent overrides
         * @param event
         */
        void focusOutEvent(QFocusEvent*event);


private:
        Q_DISABLE_COPY(EgcTextItem)
};

#endif // EGCTEXTITEM_H


