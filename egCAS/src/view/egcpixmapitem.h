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

#ifndef EgcPixmapItem_H
#define EgcPixmapItem_H

#include <QGraphicsPixmapItem>
#include <QScopedPointer>
#include "egcasiteminterface.h"
#include "egcabstractpixmapitem.h"
#include "egcabstractitem.h"

class ResizeHandle;
class EgcAbstractPixmapEntity;

class EgcPixmapItem: public QGraphicsPixmapItem, public EgcAbstractPixmapItem, public EgcAbstractItem
{
public:
        ///std constructor
        explicit EgcPixmapItem(QGraphicsItem *parent = 0);
        ///constructor for pixmap item
        EgcPixmapItem(const QPixmap & pixmap, QGraphicsItem * parent = 0);
        /// point constructor
        explicit EgcPixmapItem(const QPointF point, QGraphicsItem *parent = 0);
        ///std destructor
        virtual ~EgcPixmapItem();
        /**
         * @brief setEntity set a pointer to the entity that contains the logical structure / frontend for the view
         * @param entity a pointer to the entity that is associated with this object
         */
        void setEntity(EgcAbstractPixmapEntity* entity);
        /**
         * @brief getPosItemIface needs to be overwritten by subclasses to get the position of the item
         * @return the Position of the item
         */
        virtual QPointF getPosition( void ) const override;
        /**
         * @brief setPosItemIface needs to be overwritten by subclasses to set the position of the item
         * @param point the position to set.
         */
        virtual void setPos(const QPointF& point) override;        
        /**
         * @brief setScaleFactor set the scale factor of the pixmap
         * @param scaleFactor the scale factor to set
         */
        virtual void setScaleFactor(qreal scaleFactor) override;
        /**
         * @brief getSize returns the size of the pixmap
         * @return the size of the pixmap as QSize
         */
        virtual QSize getSize(void) override;
        /**
         * @brief setPixmap sets the pixmap to use for the item
         * @param pixmap the pixmap to set
         */
        virtual void setPixmap(QPixmap pixmap) override;
        /**
         * @brief getPixmap returns the pixmap of the current item
         * @return the pixmap of the current item
         */
        virtual QPixmap getPixmap(void) override;

protected:
        /**
         * @brief getGrid needs to be implemented by the subclasses since we cannot inherit from QGraphicsitem (the
         * subclasses already inherit from it - and we don't want to make it complicated)
         * @return the size of the grid
         */
        virtual QSizeF getGrid(void) override;        
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
        QScopedPointer<ResizeHandle> m_resizeHandle;
        bool m_childSelectionState;
        bool m_resizeHandleAdded;
        EgcAbstractPixmapEntity* m_entity;                      ///< pointer to pixmap entity
};

#endif // EgcPixmapItem_H


