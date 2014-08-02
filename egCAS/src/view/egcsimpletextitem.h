#ifndef EGCSIMPLETEXTITEM_H
#define EGCSIMPLETEXTITEM_H

#include <QGraphicsSimpleTextItem>
#include "egcasiteminterface.h"

class EgcSimpleTextItem: public QGraphicsSimpleTextItem
{
public:
        ///std constructor
        explicit EgcSimpleTextItem(QGraphicsSimpleTextItem *parent = 0);
        ///constructor for text message
        EgcSimpleTextItem(const QString & text, QGraphicsSimpleTextItem * parent = 0);
        ///std destructor
        virtual ~EgcSimpleTextItem() { }
        /**
         * @brief getPos overloads the inherited get function for the position of the item
         * @return the position of the item
         */
        virtual QPointF getPos( void ) const;
private:
        Q_DISABLE_COPY(EgcSimpleTextItem)
};

#endif // EGCSIMPLETEXTITEM_H


