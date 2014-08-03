#ifndef EGCSIMPLETEXTITEM_H
#define EGCSIMPLETEXTITEM_H

#include <QGraphicsSimpleTextItem>
#include "egcasiteminterface.h"

class EgcSimpleTextItem: public QGraphicsSimpleTextItem
{
public:
        ///std constructor
        explicit EgcSimpleTextItem(QGraphicsItem *parent = 0);
        ///constructor for text message
        EgcSimpleTextItem(const QString & text, QGraphicsItem * parent = 0);
        ///std destructor
        virtual ~EgcSimpleTextItem() { }
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

private:
        Q_DISABLE_COPY(EgcSimpleTextItem)
};

#endif // EGCSIMPLETEXTITEM_H


