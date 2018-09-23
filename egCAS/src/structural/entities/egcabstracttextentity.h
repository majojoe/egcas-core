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

#ifndef EGCABSTRACTTEXTENTITY_H
#define EGCABSTRACTTEXTENTITY_H

#include "egcabstractentity.h"

class EgcAbstractTextItem;
class QFont;

class EgcAbstractTextEntity : public EgcAbstractEntity
{
public:
        virtual ~EgcAbstractTextEntity() {}
        /**
         * @brief setItem set the formula item that is associated with this entity
         * @param item the item to set (can also be a nullptr)
         */
        virtual void setItem(EgcAbstractTextItem* item) = 0;
        /**
         * @brief getItem get the formula item that is associated with this entity
         * @param item the item that is associated with this entity (can also be a nullptr)
         */
        virtual EgcAbstractTextItem* getItem(void) = 0;
        /**
         * @brief getEntityFont returns the base font of all texts in a document
         * @return the base font of all texts
         */
        virtual QFont& getEntityFont(void) = 0;
};

#endif // EGCABSTRACTTEXTENTITY_H
