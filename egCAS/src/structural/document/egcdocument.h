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

#ifndef EGCDOCUMENT_H
#define EGCDOCUMENT_H

#include <QObject>
#include <QScopedPointer>
#include <QPointF>
#include "entities/egcentity.h"
#include "entities/egcabstractentitylist.h"
#include "entities/creator/egcformulacreator.h"
#include "entities/creator/egcpixmapcreator.h"
#include "entities/creator/egctextcreator.h"
#include "egccalculation.h"
#include "actions/egcaction.h"
#include "egcabstractdocument.h"

class EgcEntityList;
class EgCasScene;

class EgcDocument : public QObject, EgcAbstractEntityList, public EgcAbstractDocument
{
        Q_OBJECT
public:
        ///std constructor
        EgcDocument();
        /**
         * @brief getEntityList get a pointer to the top level entity list of the document
         * @return a pointer to the Entity list
         */
        EgcEntityList* getEntityList(void);
        /**
         * @brief getScene get a pointer to the scene contained by the document
         * @return a pointer to the scene of the document
         */
        EgCasScene* getScene(void);
        /**
         * @brief getCalcClass returns the calculation class in order to connect to its signals
         * @return a pointer to the EgcCalculation class
         */
        EgcCalculation const* getCalcClass(void);
        /**
         * @brief getParent returns the parent of the current list
         * @return the parent
         */
        virtual EgcAbstractEntityList* getParent(void) override;
        /**
         * @brief getDocument returns the document that contains the current list
         * @return the document
         */
        virtual EgcDocument* getDocument(void) override;
        /**
         * @brief createEntity create an entity for a list that is a sublist of this document. The list given takes
         * ownership of the created entity. The user doesn't need to worry about the pointer returned.
         * @param type the entity type to create
         * @param point the position inside the scene where to create the entity
         * @return the entity created or a nullptr if the entity couldn't be created
         */
        EgcEntity* createEntity(EgcEntityType type, QPointF point);
        /**
         * @brief cloneEntity clone an existing entity
         * @param list the list where to insert the entity to be cloned
         * @param entity2copy the entity to clone
         * @return the cloned entity
         */
        EgcEntity* cloneEntity(EgcEntityList& list, EgcEntity& entity2copy);
        /**
         * @brief deleteFormula delete a formula
         * @param formula the formula to delete
         * @return true if everything went well, false otherwise
         */
        virtual bool deleteFormula(EgcAbstractFormulaEntity* formula) override;
        /**
         * @brief deleteEntity Delete the given entity. This can be called from the entity itself.
         * @param entity the entity to delete
         */
        virtual void deleteEntity(EgcEntity* entity) override;
        /**
         * @brief resumeCalculation resume calculation after it has been stopped due to e.g. editing a formula.
         */
        virtual void resumeCalculation(void) override;
        /**
         * @brief startCalulation start the calculation of the document
         * @param entity the entity where to pause calculation
         */
        virtual void startCalulation(EgcAbstractFormulaEntity* entity = nullptr) override;
        /**
         * @brief setAutoCalculation set autocalculation on or off. This means that if a formula is changed, the kernel
         * calculates the document in the background and updates all formulas as needed.
         * @param on if true auto calculation is on ohterwise off.
         */
        void setAutoCalculation(bool on);
        /**
         * @brief getActiveFormulaEntity returns a pointer to the active Formula entity (has Focus)
         * @return pointer to active formula entity if any, or nullptr if none
         */
        EgcFormulaEntity* getActiveFormulaEntity(void);
signals:
        /**
         * @brief startDeleletingEntity signal to be emitted when entity shall be deleted
         * @param entity the entity to delete
         */
        void startDeleletingEntity(EgcEntity* entity);
        /**
         * @brief selectionChanged signal that is emitted when the selection of an item changes
         */
        void selectionChanged(void);
private slots:
        /**
         * @brief insertFormula insert a formula into the current document
         * @param point position where to insert formula
         * @param action the action to pass to the selected formula
         */
        void insertFormulaOnKeyPress(QPointF point, EgcAction action);
        /**
         * @brief deleteLaterEntity delete later the given entitiy. This can be called from the entity itself.
         * @param entity the entity to delete
         * @return true if this has been successful, false otherwise
         */
        void deleteLaterEntity(EgcEntity* entity);
        /**
         * @brief onSelectionChange is called if the selection of an item changes
         */
        void onSelectionChange(void);
private:
        virtual void sort(void) override {}
        
        QScopedPointer<EgcEntityList> m_list;           ///< the list with the items to the text, pixmap and formual items
        QScopedPointer<EgCasScene> m_scene;             ///< the scene for rendering all items
        QScopedPointer<EgcCalculation> m_calc;          ///< the class which holds all tools for doing calculations
        //QScopedPointer<EgcDocWindow> m_win;           ///< holds the window where to show the items
        EgcFormulaCreator m_formulaCreator;             ///< creator for formula entities
        EgcPixmapCreator m_pixmapCreator;               ///< creator for pixmap entities
        EgcTextCreator m_textCreator;                   ///< creator for text entities
};

#endif // EGCDOCUMENT_H
