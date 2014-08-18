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
//        void mousePressEvent(QGraphicsSceneMouseEvent * event);
//        ///do scaling of the parent here
//        void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
//        void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
        ///paint the ResizeHandle control here
        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

signals:

public slots:
private:
        QSizeF m_handleSize;
        Q_DISABLE_COPY(ResizeHandle)
};

#endif // RESIZEHANDLE_H
