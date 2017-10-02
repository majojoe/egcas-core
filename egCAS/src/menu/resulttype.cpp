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
#include <QComboBox>
#include <QToolBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include "document/egcdocument.h"
#include "resulttype.h"


ResultType::ResultType(EgcDocument* doc, QToolBar* toolbar, QWidget* parent) : QWidget(parent), m_document{doc}
{
        //add combo box for adjusting precision
        m_box = new QComboBox(parent);
        m_box->setObjectName(QStringLiteral("comboBox"));
        m_box->setFocusPolicy(Qt::NoFocus);
        setLayout(new QHBoxLayout());
        layout()->addWidget(new QLabel(QString(tr("Format:")), parent));
        layout()->addWidget(m_box);
        toolbar->addWidget(this);
        m_box->setCurrentIndex(0);
        m_box->clear();
        m_box->insertItem(0, QApplication::translate("ResultType", "standard", 0));
        m_box->insertItem(1, QApplication::translate("ResultType", "scientific", 0));
        m_box->insertItem(2, QApplication::translate("ResultType", "engineering", 0));
        m_box->insertItem(3, QApplication::translate("ResultType", "integer", 0));
        m_box->setToolTip(QApplication::translate("ResultType", "set result type of a formula", 0));
        connect(m_box, SIGNAL(currentIndexChanged(QString)), this, SLOT(resultTypeBox(QString)));
        emit m_box->currentIndexChanged(m_box->currentIndex());
        connect(m_document, &EgcDocument::selectionChanged, this, &ResultType::onSelectionChange);
}

ResultType::~ResultType()
{

}

void ResultType::setResultType(EgcNumberResultType result)
{
        EgcFormulaEntity* entity = m_document->getActiveFormulaEntity();
        if (entity) {
                entity->setNumberResultType(result);
                entity->setSelected(false);
        }
}

EgcNumberResultType ResultType::mapToType(QString text)
{
        EgcNumberResultType type = EgcNumberResultType::StandardType;

        if (text == "standard")
                type = EgcNumberResultType::StandardType;
        else if (text == "scientific")
                type = EgcNumberResultType::ScientificType;
        else if (text == "engineering")
                type = EgcNumberResultType::EngineeringType;
        else if (text == "integer")
                type = EgcNumberResultType::IntegerType;

        return type;
}

int ResultType::mapToIndex(EgcNumberResultType type)
{
        int index = 0;

        if (type == EgcNumberResultType::StandardType)
                index = 0;
        else if (type == EgcNumberResultType::ScientificType)
                index = 1;
        else if (type == EgcNumberResultType::EngineeringType)
                index = 2;
        else if (type == EgcNumberResultType::IntegerType)
                index = 3;

        return index;
}

void ResultType::resultTypeBox(QString text)
{
        EgcNumberResultType type = mapToType(text);

        setResultType(type);
}

void ResultType::onSelectionChange(void)
{
        EgcFormulaEntity* formula = m_document->getActiveFormulaEntity();
        EgcNumberResultType type;
        if (formula) {
                type = formula->getNumberResultType();
        }

        //set number of digits
        m_box->blockSignals(true);
        m_box->setCurrentIndex(mapToIndex(type));
        m_box->blockSignals(false);
}
