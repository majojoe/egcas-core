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

#include <QMessageBox>
#include <QGraphicsView>
#include "egcasscene.h"
#include "../entities/egcentitylist.h"
#include "egcdocument.h"
#include "entities/egcentitylist.h"
#include "view/egcasscene.h"
#include "egccalculation.h"
#include "entities/egcformulaentity.h"
#include "view/egcformulaitem.h"
#include "entities/egctextentity.h"
#include "view/egctextitem.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

EgcDocument::EgcDocument() : m_list{new EgcEntityList(this)}, m_scene{new EgCasScene(*this, nullptr)}, m_calc{new EgcCalculation()}
{
        connect(m_scene.data(), SIGNAL(createFormula(QPointF, EgcAction)), this, SLOT(insertFormulaOnKeyPress(QPointF, EgcAction)));
        connect(this, SIGNAL(startDeleletingEntity(EgcEntity*)), this, SLOT(deleteLaterEntity(EgcEntity*)), Qt::QueuedConnection);
        connect(m_scene.data(), &EgCasScene::selectionChanged, this, &EgcDocument::selectionChanged);
        connect(m_calc.data(), SIGNAL(errorOccurred(EgcKernelErrorType,QString)),  this, SLOT(handleKernelMessages(EgcKernelErrorType,QString)));
}

EgcEntityList* EgcDocument::getEntityList(void)
{
        return m_list.data();
}

EgCasScene* EgcDocument::getScene(void) const
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
        EgcFormulaCreator formulaCreator;             ///< creator for formula entities
        EgcPixmapCreator pixmapCreator;               ///< creator for pixmap entities
        EgcTextCreator textCreator;                   ///< creator for text entities

        EgcEntity* ptr = nullptr;

        switch (type) {
        case EgcEntityType::Text:
                ptr = textCreator.create(*m_list, point);
                break;
        case EgcEntityType::Picture:
                ptr = pixmapCreator.create(*m_list, point);
                break;
        case EgcEntityType::Formula:
                ptr = formulaCreator.create(*m_list, point);
        }

        return ptr;
}

EgcEntity* EgcDocument::cloneEntity(EgcEntityList& list, EgcEntity& entity2copy)
{
        EgcFormulaCreator formulaCreator;             ///< creator for formula entities
        EgcPixmapCreator pixmapCreator;               ///< creator for pixmap entities
        EgcTextCreator textCreator;                   ///< creator for text entities
        EgcEntity* ptr = nullptr;

        EgcEntityType type = entity2copy.getEntityType();

        switch (type) {
        case EgcEntityType::Text:
                ptr = textCreator.clone(list, entity2copy);
                break;
        case EgcEntityType::Picture:
                ptr = pixmapCreator.clone(list, entity2copy);
                break;
        case EgcEntityType::Formula:
                ptr = formulaCreator.clone(list, entity2copy);
        }

        return ptr;
}

EgcCalculation const* EgcDocument::getCalcClass(void)
{
        return m_calc.data();
}

void EgcDocument::insertFormulaOnKeyPress(QPointF point, EgcAction action)
{
        EgcFormulaEntity *formula = static_cast<EgcFormulaEntity*>(createEntity(EgcEntityType::Formula, point));
        formula->setSelected(true);
        formula->handleAction(action);
}

bool EgcDocument::deleteFormula(EgcAbstractFormulaEntity* formula)
{
        if(!formula)
                return false;
        EgcFormulaEntity* entity = static_cast<EgcFormulaEntity*>(formula);

        //inform calculation about deleting an entity
        m_calc->deleteEntity(entity);

        EgcAbstractFormulaItem* item = entity->getItem();
        if (!item)
                return false;
        this->m_scene->deleteItem(item);

        m_list->deleteEntity(entity);

        return true;
}

void EgcDocument::deleteEntity(EgcEntity* entity)
{
        emit startDeleletingEntity(entity);
}

void EgcDocument::deleteAll()
{
        //reset calculation
        m_calc->reset();
        m_scene->clear();
        m_list->deleteAll();
}

void EgcDocument::deleteLaterEntity(EgcEntity* entity)
{
        if (entity->getEntityType() == EgcEntityType::Formula) {
                deleteFormula(dynamic_cast<EgcAbstractFormulaEntity*>(entity));
        } else {
                if (entity) {
                        m_list->deleteEntity(entity);
                }
        }
}

void EgcDocument::resumeCalculation(void)
{
        if (m_calc.isNull())
                return;

        m_calc->resumeCalculation();
}

void EgcDocument::restartCalculation(void)
{
        if (m_calc.isNull())
                return;

        m_calc->restart();
}

void EgcDocument::startCalulation(EgcAbstractFormulaEntity* entity)
{
        if (m_calc.isNull())
                return;

        (void) m_calc->calculate(*m_list, true, entity);
}

void EgcDocument::setAutoCalculation(bool on)
{
        if (m_calc.isNull())
                return;

        m_calc->setAutoCalculation(on);
}

EgcFormulaEntity* EgcDocument::getActiveFormulaEntity(void)
{
        QList<QGraphicsItem*> list = m_scene->selectedItems();
        if (list.empty())
                return nullptr;
        QGraphicsItem *item = list.at(0);
        if (!item)
                return nullptr;

        EgcFormulaItem* formulaItem = dynamic_cast<EgcFormulaItem*>(item);
        if (!formulaItem)
                return nullptr;

        EgcAbstractFormulaEntity *aFormulaEntity = formulaItem->getEnity();
        if (!aFormulaEntity)
                return nullptr;
        EgcFormulaEntity* entity = dynamic_cast<EgcFormulaEntity*>(aFormulaEntity);
        if (!entity)
                return nullptr;

        return entity;
}

EgcTextEntity* EgcDocument::getActiveTextEntity()
{
        QList<QGraphicsItem*> list = m_scene->selectedItems();
        if (list.empty())
                return nullptr;
        QGraphicsItem *item = list.at(0);
        if (!item)
                return nullptr;

        EgcTextItem* textItem = dynamic_cast<EgcTextItem*>(item);
        if (!textItem)
                return nullptr;

        EgcAbstractTextEntity *aTextEntity = textItem->getEnity();
        if (!aTextEntity)
                return nullptr;
        EgcTextEntity* entity = dynamic_cast<EgcTextEntity*>(aTextEntity);
        if (!entity)
                return nullptr;

        return entity;
}

void EgcDocument::onSelectionChange(void)
{
        emit selectionChanged();
}

void EgcDocument::handleKernelMessages(EgcKernelErrorType type, QString message)
{
        (void) type;

        QMessageBox msgBox;
        msgBox.setText(tr("Kernel warning"));
        msgBox.setInformativeText(message);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        (void) msgBox.exec();
}

QPointF EgcDocument::getLastCursorPosition(void)
{
        return m_scene.data()->getLastCursorPositon();
}

QSizeF EgcDocument::getMaxItemSize(QPointF point) const
{
        return m_scene.data()->worksheet().getMaxItemSize(point);
}

void EgcDocument::updateView()
{
        QGraphicsView* view = m_scene->views().at(0);
        if (view) {
                view->viewport()->update();
        }
}

void EgcDocument::saveToFile(QString filename)
{
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QXmlStreamWriter stream(&file);
        serialize(stream);

        file.close();
}

void EgcDocument::readFromFile(QString filename)
{
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return;

        QXmlStreamReader stream(&file);
        deserialize(stream, 0);

        file.close();
}

void EgcDocument::serialize(QXmlStreamWriter& stream)
{
        stream.setAutoFormatting(true);
        stream.writeStartDocument();

        stream.writeStartElement("document");
        stream.writeAttribute("width", QString("%1").arg(getWidth()));
        stream.writeAttribute("height", QString("%1").arg(getHeight()));
        stream.writeAttribute("version", QString(EGCAS_VERSION));

        EgcEntityList* list = getEntityList();
        QMutableListIterator<EgcEntity*> iter = list->getIterator();
        while(iter.hasNext()) {
                iter.next()->serialize(stream);
        }

        stream.writeEndElement(); // document

        stream.writeEndDocument();
}

void EgcDocument::deserialize(QXmlStreamReader& stream, quint32 version)
{
        version = 0;

        if (stream.readNextStartElement()) {
                if (stream.name() == QLatin1String("document")) {
                        QXmlStreamAttributes attr = stream.attributes();
                        QString ver = attr.value("version").toString();
                        QStringList list = ver.split('.');
                        if (list.size() == 3) {
                                version = list.at(0).toUInt() << 16;
                                version += list.at(1).toUInt() << 8;
                                version += list.at(2).toUInt();
                        }
                        if (    attr.hasAttribute("height") && attr.hasAttribute("width")
                             && (version == 2 || version == 3)) {
                                qreal height = attr.value("height").toFloat();
                                qreal width = attr.value("width").toFloat();
                                setWidth(width);
                                setHeight(height);
                                //delete all contents from the document
                                deleteAll();

                                EgcEntity* entity = nullptr;
                                while (stream.readNextStartElement()) {
                                        entity = nullptr;
                                        if (stream.name() == QLatin1String("text_entity"))
                                                entity = createEntity(EgcEntityType::Text);
                                        else if (stream.name() == QLatin1String("pic_entity"))
                                                entity = createEntity(EgcEntityType::Picture);
                                        else
                                                stream.skipCurrentElement();

                                        if (entity)
                                                entity->deserialize(stream, version);
                                }
                        } else {
                                stream.raiseError(QObject::tr("This file version is not supported. Maybe saved by a newer version."));
                        }
                } else {
                        stream.raiseError(QObject::tr("The file is not an egcas file."));
                }
        }
}

void EgcDocument::setHeight(qreal height)
{
        QRectF rct = getScene()->sceneRect();
        rct.setHeight(height);
        getScene()->setSceneRect(rct);
}

qreal EgcDocument::getHeight() const
{
        return getScene()->sceneRect().height();
}

void EgcDocument::setWidth(qreal width)
{
        QRectF rct = getScene()->sceneRect();
        rct.setWidth(width);
        getScene()->setSceneRect(rct);
}

qreal EgcDocument::getWidth() const
{
        return getScene()->sceneRect().width();
}
