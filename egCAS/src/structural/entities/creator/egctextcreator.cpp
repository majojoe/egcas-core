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

#include <QScopedPointer>
#include "egctextcreator.h"
#include "entities/egcentitylist.h"
#include "entities/egctextentity.h"
#include "view/egcasiteminterface.h"
#include "view/egcasscene.h"
#include "document/egcdocument.h"
#include "egctextitem.h"

EgcTextCreator::EgcTextCreator()
{

}

EgcTextCreator::~EgcTextCreator()
{

}

EgcEntity* EgcTextCreator::create(EgcEntityList& list, QPointF point)
{
        QScopedPointer<EgcTextEntity> entity(new EgcTextEntity());
        if (entity.isNull())
                return nullptr;
        EgcDocument* doc = list.getDocument();
        EgCasScene* scene = doc->getScene();
        if (scene->addText(*entity, point)) {
                list.addEntity(entity.data());
                QFont font_text(entity->getGenericFont());
                entity->setFont(font_text);
                return entity.take();
        }

        return nullptr;
}

EgcEntity* EgcTextCreator::clone(EgcEntityList& list, EgcEntity& entity2copy)
{
        if (entity2copy.getEntityType() != EgcEntityType::Text)
                return nullptr;
        EgcTextEntity& entityCopyRef = static_cast<EgcTextEntity&>(entity2copy);
        QScopedPointer<EgcTextEntity> entity(new EgcTextEntity(entityCopyRef));
        if (entity.isNull())
                return nullptr;
        EgcDocument* doc = list.getDocument();
        EgCasScene* scene = doc->getScene();
        if (scene->addText(*entity, entity2copy.getPosition())) {
                list.addEntity(entity.data());
                //set the item properties
                entity->setFont(entityCopyRef.getFont());
                entity->setText(entityCopyRef.getText());

                return entity.take();
        }

        return nullptr;
}

