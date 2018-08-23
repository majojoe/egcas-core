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

#ifndef RESIZEHANDLE_H
#define RESIZEHANDLE_H

#include <QGraphicsItem>

class ResizeHandle : public QGraphicsItem
{
public:
        ///std constructor
        explicit ResizeHandle(QGraphicsItem *content, const QSizeF&size);
        ///std destructor
        virtual ~ResizeHandle();
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
        /**
         * @brief itemChangeInfo informs this handle about item changes of the m_resizableContent item to stay
         * informed about its state. This must be called in the itemChange method of m_resizableContent.
         * @param change enum that describes state changes that are notified
         * @param value the value that has changed
         * @param content_scene the scene of which m_resizableContent belongs to
         * @return the value that has been adjusted
         */
        void itemChangeInfo(GraphicsItemChange change, const QVariant &value, QGraphicsScene* content_scene);
        /**
         * @brief mouseReleaseEventInfo informs this handle object about a mouse release event in m_resizableContent
         * This must be called after the mouse handle of the base class has been called!!!
         */
        void mouseReleaseEventInfo(void);
        /**
         * @brief mouseMoveEventInfo informs this handle object about a mouse move event in m_resizableContent
         * This must be called after the mouse handle of the base class has been called!!!
         */
        void mouseMoveEventInfo(void);
        /**
         * @brief activate the resize handle
         */
        void activate(void);

signals:

public slots:
private:
        QGraphicsItem *m_resizableContent;
        QSizeF m_handleSize;
        ///This is the position where the scaling starts. Thats needed to be able to calculate scaling.
        QPointF m_scaleStartPos;        ///< start position of this handle (mouse move) when starting resizing
        qreal m_contentStartScale;      ///< start scale of m_resizeableContent when starting resizing
        qreal m_contentStartDiag;       ///< start diagonal of m_resizeableContent when starting resizing
        bool m_addedToScene;            ///< variable that holds if this item has already been added to the scene of the scene of m_resizableContent
        bool m_selectionState;          ///< the selection state of m_resizableContent

        Q_DISABLE_COPY(ResizeHandle)

};

#endif // RESIZEHANDLE_H
