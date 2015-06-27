/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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

#ifndef EGCTEXTITEM_H
#define EGCTEXTITEM_H

#include <QGraphicsTextItem>
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
signals:


private:
        Q_DISABLE_COPY(EgcTextItem)
};

#endif // EGCTEXTITEM_H


