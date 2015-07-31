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
#include "egcformulacreator.h"
#include "entities/egcentitylist.h"
#include "entities/egcformulaentity.h"
#include "view/egcasiteminterface.h"
#include "view/egcasscene.h"
#include "document/egcdocument.h"
#include "egcformulaitem.h"

EgcFormulaCreator::EgcFormulaCreator()
{
}

EgcFormulaCreator::~EgcFormulaCreator()
{
}

EgcEntity* EgcFormulaCreator::create(EgcEntityList* list, QPointF point)
{
        QScopedPointer<EgcFormulaEntity> entity(new EgcFormulaEntity());
        if (entity.isNull())
                return nullptr;
        EgcDocument* doc = list->getDocument();
        EgCasScene* scene = doc->getScene();
        if (scene->addFormula(*entity, point)) {
                list->addEntity(entity.data());
                return entity.take();
        }

        return nullptr;
}

EgcEntity* EgcFormulaCreator::copy(EgcEntityList* list, EgcEntity& entity2copy)
{
        if (entity2copy.getEntityType() != EgcEntityType::Formula)
                return nullptr;
        QScopedPointer<EgcFormulaEntity> entity(new EgcFormulaEntity(static_cast<EgcFormulaEntity&>(entity2copy)));
        if (entity.isNull())
                return nullptr;
        EgcDocument* doc = list->getDocument();
        EgCasScene* scene = doc->getScene();
        EgcFormulaItem* item = scene->addFormula(*entity, entity2copy.getPositon());
        if (item) {
                item->setFontSize(static_cast<EgcFormulaEntity&>(entity2copy).getFontSize());
                list->addEntity(entity.data());

                return entity.take();
        }

        return nullptr;
}
