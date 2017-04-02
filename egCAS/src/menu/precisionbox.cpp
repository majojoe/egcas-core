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
#include "document/egcdocument.h"
#include "entities/egcformulaentity.h"
#include "precisionbox.h"


#define MAP_COMBO_TO_PRECISION(prec)  ((prec == 0) ? 0 : (prec + 1))
#define MAP_PRECISION_TO_COMBO(prec)  ((prec == 0) ? 0 : (prec - 1))

PrecisionBox::PrecisionBox(EgcDocument* doc, QToolBar* toolbar, QWidget* parent) : QWidget(parent), m_document{doc}
{
        //add combo box for adjusting precision
        m_box = new QComboBox(this);
        m_box->setObjectName(QStringLiteral("comboBox"));
        m_box->setFocusPolicy(Qt::NoFocus);
        toolbar->addWidget(m_box);
        m_box->setCurrentIndex(0);
        m_box->clear();
        m_box->insertItem(0, QApplication::translate("PrecisionBox", "std", 0));
        m_box->insertItem(1, QApplication::translate("PrecisionBox", "2", 0));
        m_box->insertItem(2, QApplication::translate("PrecisionBox", "3", 0));
        m_box->insertItem(3, QApplication::translate("PrecisionBox", "4", 0));
        m_box->insertItem(4, QApplication::translate("PrecisionBox", "5", 0));
        m_box->insertItem(5, QApplication::translate("PrecisionBox", "6", 0));
        m_box->insertItem(6, QApplication::translate("PrecisionBox", "7", 0));
        m_box->insertItem(7, QApplication::translate("PrecisionBox", "8", 0));
        m_box->insertItem(8, QApplication::translate("PrecisionBox", "9", 0));
        m_box->insertItem(9, QApplication::translate("PrecisionBox", "10", 0));
        m_box->insertItem(10, QApplication::translate("PrecisionBox", "11", 0));
        m_box->insertItem(11, QApplication::translate("PrecisionBox", "12", 0));
        m_box->insertItem(12, QApplication::translate("PrecisionBox", "13", 0));
        m_box->insertItem(13, QApplication::translate("PrecisionBox", "14", 0));
        m_box->insertItem(14, QApplication::translate("PrecisionBox", "15", 0));
        m_box->insertItem(15, QApplication::translate("PrecisionBox", "16", 0));
        m_box->setToolTip(QApplication::translate("PrecisionBox", "set number of significant digits", 0));
        connect(m_box, SIGNAL(currentIndexChanged(int)), this, SLOT(precBox(int)));
        emit m_box->currentIndexChanged(m_box->currentIndex());
        connect(m_document, &EgcDocument::selectionChanged, this, &PrecisionBox::onSelectionChange);
}

PrecisionBox::~PrecisionBox()
{

}

void PrecisionBox::setPrecision(int prec)
{
        EgcFormulaEntity* entity = m_document->getActiveFormulaEntity();
        if (entity) {
                entity->setNumberOfSignificantDigits(prec);
                entity->setSelected(false);
        } else {
                EgcFormulaEntity::setStdNrSignificantDigis(prec);
                m_document->startCalulation();
        }
}

void PrecisionBox::precBox(int prec)
{
        setPrecision(MAP_COMBO_TO_PRECISION(prec));
}

void PrecisionBox::onSelectionChange(void)
{
        EgcFormulaEntity* formula = m_document->getActiveFormulaEntity();
        quint8 digits;
        if (formula) {
                digits = formula->getNumberOfSignificantDigits();
        } else {
                digits = EgcFormulaEntity::getStdNrSignificantDigis();
        }

        //set number of digits
        m_box->blockSignals(true);
        m_box->setCurrentIndex(MAP_PRECISION_TO_COMBO(digits));
        m_box->blockSignals(false);
}
