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
#include <QSpinBox>
#include <QToolBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include "document/egcdocument.h"
#include "mathfont.h"
#include "entities/egcformulaentity.h"


MathFont::MathFont(EgcDocument* doc, QToolBar* toolbar, QWidget* parent) : QWidget(parent), m_document{doc}
{
        //add combo box for adjusting precision
        m_box = new QSpinBox (parent);
        m_box->setObjectName(QStringLiteral("spinBox"));
        setLayout(new QHBoxLayout());
        layout()->addWidget(new QLabel(QString(tr("Size:")), parent));
        layout()->addWidget(m_box);
        toolbar->addWidget(this);
        m_box->setValue(EgcFormulaEntity::getGenericFontSize());
        m_box->setToolTip(QApplication::translate("MathFont", "set font size of formulas", 0));
        connect(m_box, SIGNAL(valueChanged(int)), this, SLOT(changeSize(int)));
}

MathFont::~MathFont()
{

}

void MathFont::changeSize(int size)
{
        EgcFormulaEntity::setGenericFontSize(size);
}
