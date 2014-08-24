#ifndef EgcPixmapItem_H
#define EgcPixmapItem_H

#include <QGraphicsPixmapItem>
#include "egcasiteminterface.h"

class ResizeHandle;

class EgcPixmapItem: public QGraphicsPixmapItem
{
public:
        ///std constructor
        explicit EgcPixmapItem(QGraphicsItem *parent = 0);
        ///constructor for pixmap item
        EgcPixmapItem(const QPixmap & pixmap, QGraphicsItem * parent = 0);
        ///std destructor
        virtual ~EgcPixmapItem();
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
         * @brief mousePressEvent overrides mouseReleaseEvent from QGraphicsItem
         * @param event pointer to QGraphicsSceneMouseEvent
         */
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        /**
         * @brief mousePressEvent overrides mouseMoveEvent from QGraphicsItem
         * @param event pointer to QGraphicsSceneMouseEvent
         */
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:


private:
        Q_DISABLE_COPY(EgcPixmapItem)
        ResizeHandle *m_resizeHandle;
        bool m_childSelectionState;
        bool m_resizeHandleAdded;
};

#endif // EgcPixmapItem_H


