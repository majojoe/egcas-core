/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#ifndef EGCCROSSITEM_H
#define EGCCROSSITEM_H

#include <QGraphicsItem>

class EgcCrossItem : public QGraphicsItem
{
public:
        ///std constructor
        explicit EgcCrossItem(QGraphicsItem *parent = Q_NULLPTR);
        ///std destructor
        virtual ~EgcCrossItem() {}
        ///set the bounding rect for the EgcCrossItem
        QRectF boundingRect() const;
        ///paint the EgcCrossItem control here
        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
        /**
         * @brief up moves the cross cursor up
         */
        void up(void);
        /**
         * @brief snap to next grid increment upwards starting at yCoord
         * @param yCoord the coordinate where to start with snapping upwards -> direction 0
         */
        void up(qreal yCoord);
        /**
         * @brief up moves the cross cursor down
         */
        void down(void);
        /**
         * @brief snap to next grid increment downwards starting at yCoord
         * @param yCoord the coordinate where to start with snapping downwards -> direction infinity
         */
        void down(qreal yCoord);
        /**
         * @brief up moves the cross cursor left
         */
        void left(void);
        /**
         * @brief snap to next grid increment to the left starting at xCoord
         * @param xCoord the coordinate where to start with snapping to the left -> direction 0
         */
        void left(qreal xCoord);
        /**
         * @brief up moves the cross cursor right
         */
        void right(void);
        /**
         * @brief snap to next grid increment to the right starting at xCoord
         * @param xCoord the coordinate where to start with snapping to the right -> direction infinity
         */
        void right(qreal xCoord);

protected:
        /**
         * @brief itemChange reimplements change function of QGraphicsItem to be able to realize a grid
         * @param change enum that describes state changes that are notified
         * @param value the value that has changed
         * @return the value that has been adjusted
         */
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
        /**
         * @brief keyReleaseEvent overwrites key events
         * @param keyEvent the key event to react on
         */
        virtual void keyPressEvent(QKeyEvent *keyEvent) override;
        /**
         * @brief focusOutEvent overwrite focus in event
         * @param event focus event
         */
        virtual void focusOutEvent(QFocusEvent * event) override;
        /**
         * @brief focusOutEvent overwrite focus in event
         * @param event focus event
         */
        virtual void focusInEvent(QFocusEvent * event) override;

signals:

public slots:
private:
        /**
         * @brief snapGrid snap to scene grid
         * @param pos the position to snap to grid
         * @return the new position that is snapped to the grid
         */
        QPointF snapGrid(const QPointF& pos);
        /**
         * @brief getGrid needs to be implemented by the subclasses since we cannot inherit from QGraphicsitem (the
         * subclasses already inherit from it - and we don't want to make it complicated)
         * @return the size of the grid
         */
        QSizeF getGrid(void);

        const QSize m_size = QSize(40,40);     ///< size of the cross

        Q_DISABLE_COPY(EgcCrossItem)

};

#endif // #ifndef EGCCROSSITEM_H
