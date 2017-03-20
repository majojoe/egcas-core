/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
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

#ifndef EGCENTITYLIST_H
#define EGCENTITYLIST_H

#include <QList>
#include "egcentity.h"
#include "egcabstractentitylist.h"

class EgcDocument;

/**
 * @brief The EgcEntityList class is a list that holds formulas, text and picture items
 */
class EgcEntityList : public EgcAbstractEntityList
{
public:
        /// std constructor
        EgcEntityList(EgcAbstractEntityList* parent = nullptr);
        /// std destructor
        ~EgcEntityList();

        /// sort the list
        void sort(void) override;
        /**
         * @brief addEntity add a entity (formula, text or picture) to the list. The list takes ownership of the entity
         * @param entity a pointer to the entity to add
         */
        void addEntity(EgcEntity* entity);
        /**
         * @brief deleteEntity removes the given entity from the list and deletes it (since the list has the ownership).
         * @param entity the entity to delete
         * @return true if all was fine, false otherwise
         */
        bool deleteEntity(EgcEntity* entity);
        /**
         * @brief takeEntity removes the given entity from the list and returns it (the entity is not deleted). The
         * the caller takes the ownership of the entity!!!
         * @param entity the entiy to remove from list.
         * @return the entity removed from the list.
         */
        EgcEntity* takeEntity(EgcEntity* entity);
        /**
         * @brief toStart go to the begin of the list and start from there (when calling next)
         */
        void toStart(void);
        /**
         * @brief next returns always the next element in the list until a nullptr is returned. This marks the end of
         * the list
         * @return nullptr if the end is reached, otherwise the next element in the list.
         */
        EgcEntity* next(void);
        /**
         * @brief getDocument returns a pointer to the document this list is in (as child or subchild)
         * @return a pointer to the document, if nullptr is returned the list is not included in a document
         */
        virtual EgcDocument* getDocument(void) override;
        /**
         * @brief getParent returns the parent of the current list
         * @return the parent
         */
        virtual EgcAbstractEntityList* getParent(void) override;
        /**
         * @brief createEntity creates an entity for the given list
         * @param type the type of entity to create
         * @param point the position at which to create the entity (on the scene)
         * @return the entity created, or a nullptr if no entity could be created
         */
        EgcEntity* createEntity(EgcEntityType type, QPointF point);
        /**
         * @brief cloneEntity copy a given entity including all of its subsequent elements
         * @param entity entity to copy
         * @return the created entity
         */
        EgcEntity* cloneEntity(EgcEntity& entity2copy);
        /**
         * @brief getIterator returns an iterator that operates on the enity list
         * @return returns a list iterator
         */
        QMutableListIterator<EgcEntity*> getIterator(void);

private:
        QList<EgcEntity*> m_list;               ///< holds a bunch of entities of a document
        int m_index;
        EgcAbstractEntityList* m_parent;        ///< pointer to the parent containing the this list
};

#endif // EGCENTITYLIST_H
