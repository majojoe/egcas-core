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

#include "egcasscene.h"
#include "../entities/egcentitylist.h"
#include "egcdocument.h"
#include "entities/egcentitylist.h"
#include "view/egcasscene.h"
#include "egccalculation.h"
#include "entities/egcformulaentity.h"

EgcDocument::EgcDocument() : m_list{new EgcEntityList()}, m_scene{new EgCasScene(nullptr, this)}, m_calc{new EgcCalculation()}
{
        connect(m_scene.data(), SIGNAL(createFormula(QPointF, EgcAction)), this, SLOT(insertFormulaOnKeyPress(QPointF, EgcAction)));
}

EgcEntityList* EgcDocument::getEntityList(void)
{
        return m_list.data();
}

EgCasScene* EgcDocument::getScene(void)
{
        return m_scene.data();
}

EgcAbstractEntityList* EgcDocument::getParent(void)
{
        return nullptr;
}

EgcDocument* EgcDocument::getDocument(void)
{
        return this;
}

EgcEntity* EgcDocument::createEntity(EgcEntityType type, QPointF point)
{
        EgcEntity* ptr = nullptr;

        switch (type) {
        case EgcEntityType::Text:
                ptr = m_textCreator.create(*m_list, point);
                break;
        case EgcEntityType::Picture:
                ptr = m_pixmapCreator.create(*m_list, point);
                break;
        default:
                ptr = m_formulaCreator.create(*m_list, point);
        }

        return ptr;
}

EgcEntity* EgcDocument::cloneEntity(EgcEntityList& list, EgcEntity& entity2copy)
{
        EgcEntity* ptr = nullptr;

        EgcEntityType type = entity2copy.getEntityType();

        switch (type) {
        case EgcEntityType::Text:
                ptr = m_textCreator.clone(list, entity2copy);
                break;
        case EgcEntityType::Picture:
                ptr = m_pixmapCreator.clone(list, entity2copy);
                break;
        default:
                ptr = m_formulaCreator.clone(list, entity2copy);
        }

        return ptr;
}

void EgcDocument::calculate(void)
{
        m_calc->calculate(*m_list);
}

EgcCalculation const* EgcDocument::getCalcClass(void)
{
        return m_calc.data();
}

void EgcDocument::insertFormulaOnKeyPress(QPointF point, EgcAction action)
{
        EgcFormulaEntity *formula = static_cast<EgcFormulaEntity*>(createEntity(EgcEntityType::Formula, point));
        formula->setSelected();
        formula->handleAction(action);
}

bool EgcDocument::deleteFormula(EgcAbstractFormulaEntity* formula)
{
#warning implement this
}
