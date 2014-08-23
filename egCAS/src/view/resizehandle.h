#ifndef RESIZEHANDLE_H
#define RESIZEHANDLE_H

#include <QGraphicsItem>

class ResizeHandle : public QGraphicsItem
{
public:
        ///std constructor
        explicit ResizeHandle(const QSizeF&size, QGraphicsItem *parent = 0);
        ///std destructor
        virtual ~ResizeHandle() {}
        ///set the bounding rect for the ResizeHandle
        QRectF boundingRect() const;
        void mousePressEvent(QGraphicsSceneMouseEvent * event);
        ///do scaling of the parent here
        void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
        ///paint the ResizeHandle control here
        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
        /**
         * @brief itemChange reimplements change function of QGraphicsItem to be able to change the cursor shape
         * @param change enum that describes state changes that are notified
         * @param value the value that has changed
         * @return the value that has been adjusted
         */
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);


signals:

public slots:
private:
        QSizeF m_handleSize;
        Q_DISABLE_COPY(ResizeHandle)
        ///This is the position where the scaling starts. Thats needed to be able to calculate scaling.
        QPointF m_scaleStartPos;
        QGraphicsItem *m_parent;
};

#endif // RESIZEHANDLE_H
