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

#ifndef EGCTEXTENTITY_H
#define EGCTEXTENTITY_H

#include <QString>
#include "egcentity.h"
#include "egcabstracttextitem.h"

class QPointF;
class EgcTextItem;


/**
 * @brief The EgcTextEntity class is a class for handling text entities
 */
class EgcTextEntity : public EgcEntity
{
public:
        /**
         * @brief EgcTextEntity std constructor
         */
        EgcTextEntity(void);
        ///std destructor
        virtual ~EgcTextEntity();
        /**
         * @brief getEntityType returns the entity type of the current class, needs to be reimplemented in a subclass
         * @return the entity type
         */
        virtual enum EgcEntityType getEntityType(void) const;
        /**
         * @brief getPositon returns the position of the current entity
         * @return the position of the entity in the current worksheet
         */
        virtual QPointF getPositon(void) const;
        /**
         * @brief setPosition set the position of a entity
         * @param pos the position where the entity should be
         */
        virtual void setPosition(QPointF pos) override;
        /**
         * @brief getText returns the text hold by this entity/item
         * @return the text hold
         */
        QString getText(void) const;
        /**
         * @brief setText set the text of a text entity
         * @param text the text to set
         */
        void setText(QString text);
        /**
         * @brief getFont returns the font used by this entity/item
         * @return the font used
         */
        QFont getFont(void) const;
        /**
         * @brief setFont set the font of this entity
         * @param font the font to set
         */
        void setFont(QFont font);
        /**
         * @brief setItem set the formula item that is associated with this entity
         * @param item the item to set (can also be a nullptr)
         */
        void setItem(EgcAbstractTextItem* item);

private:
        EgcAbstractTextItem *m_item;                    ///< pointer to QGraphicsitem hold by scene
};

#endif // EGCTEXTENTITY_H
