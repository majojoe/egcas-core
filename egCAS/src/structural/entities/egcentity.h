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

#ifndef EGCENTITY_H
#define EGCENTITY_H

class QPointF;
class EgcAbstractEntityList;

/**
 * @brief The EgcEntityType defines different types of entities inside a egCAS (formula) document/worksheet
 */
enum class EgcEntityType
{
        Formula = 0,        ///< this is a formula int the document
        Picture,            ///< this is a picture in the document
        Text                ///< this is a text element in the document
};

/**
 * @brief The EgcEntity class is a abstract base class for all types of entities in a document (text, formulas, pictures, ...)
 */
class EgcEntity
{
public:
        /**
         * @brief EgcEntity std constructor
         */
        EgcEntity(void);
        ///std destructor
        virtual ~EgcEntity();
        /**
         * @brief getEntityType returns the entity type of the current class, needs to be reimplemented in a subclass
         * @return the entity type
         */
        virtual enum EgcEntityType getEntityType(void) const = 0;
        /**
         * @brief getPositon returns the position of the current entity
         * @return the position of the entity in the current worksheet
         */
        virtual QPointF getPosition(void) const = 0;
        /**
         * @brief setPosition set the position of a entity
         * @param pos the position where the entity should be
         */
        virtual void setPosition(QPointF pos) = 0;
        /**
         * @brief operator < overloads the "<" operator to be able to sort the entities in the order they are inserted
         * on the document sheet
         * @param rhs the entity to compare against
         * @return true if condition is true, false otherwise
         */
        bool operator<(const EgcEntity& rhs) const;
        /**
         * @brief setList set the entity list that contains this entity
         * @param list the list make this item point to
         */
        void setList(EgcAbstractEntityList* list);
        /**
         * @brief lessThan compares two entities with each other
         * @param o1 entity 1 to compare to entity o2
         * @param o2 entity 2 to compare to entity o1
         * @return true if o1 is less than 02, false otherwise
         */
        static bool lessThan(EgcEntity* o1, EgcEntity* o2);
protected:
        EgcAbstractEntityList* m_list;            ///< pointer to the list containing this entity
};

#endif // EGCENTITY_H
