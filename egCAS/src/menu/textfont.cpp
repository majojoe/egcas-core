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
#include <QFontComboBox>
#include "document/egcdocument.h"
#include "textfont.h"
#include "entities/egctextentity.h"


TextFont::TextFont(EgcDocument* doc, QToolBar* toolbar, QWidget* parent) : QWidget(parent), m_document{doc}
{
        //add combo box for adjusting text size
        m_box = new QSpinBox (parent);
        m_box->setObjectName(QStringLiteral("spinBox"));
        m_fontBox = new QFontComboBox(parent);
        m_fontBox->setObjectName(QStringLiteral("fontComboBox"));
        setLayout(new QHBoxLayout());
        layout()->addWidget(new QLabel(QString(tr("Text:")), parent));
        layout()->addWidget(m_box);
        layout()->addWidget(m_fontBox);
        toolbar->addWidget(this);
        int pointSize = EgcTextEntity::getGenericFont().pointSize();
        m_box->setValue(pointSize);
        m_box->setToolTip(QApplication::translate("TextFont", "set font size of text", 0));
        m_fontBox->setToolTip(QApplication::translate("TextFont", "set font of text", 0));
        m_fontBox->setCurrentFont(EgcTextEntity::getGenericFont());
        connect(m_box, SIGNAL(valueChanged(int)), this, SLOT(changeSize(int)));
        connect(m_fontBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(changeFont(QFont)));
        connect(m_document, &EgcDocument::selectionChanged, this, &TextFont::onSelectionChange);
}

TextFont::~TextFont()
{

}

void TextFont::changeSize(int size)
{
        QFont font;
        EgcAbstractTextEntity *entity;
        entity = m_document->getActiveTextEntity();
        if (entity)
                font = m_document->getActiveTextEntity()->getFont();
        else
                font = EgcTextEntity::getGenericFont();
        font.setPointSize(size);
        if (entity) {
                EgcTextEntity* text = static_cast<EgcTextEntity*>(entity);
                text->setFont(font);
        } else {
                EgcTextEntity::setGenericFont(font);
        }
        m_document->updateView();
}

void TextFont::changeFont(QFont font)
{
        QFont oldFont;
        EgcAbstractTextEntity *entity;
        entity = m_document->getActiveTextEntity();
        if (entity)
                oldFont = m_document->getActiveTextEntity()->getFont();
        else
                oldFont = EgcTextEntity::getGenericFont();
        oldFont.setFamily(font.family());
        if (entity) {
                EgcTextEntity* text = static_cast<EgcTextEntity*>(entity);
                text->setFont(oldFont);
        } else {
                EgcTextEntity::setGenericFont(oldFont);
        }
        m_document->updateView();
}

void TextFont::onSelectionChange()
{
        EgcTextEntity* text = m_document->getActiveTextEntity();
        QFont font;
        if (text) {
                font = text->getFont();
        } else {
                font = EgcTextEntity::getGenericFont();
        }
        //set font properties
        m_box->blockSignals(true);
        m_box->setValue(font.pointSize());
        m_fontBox->setCurrentFont(font);
        m_box->blockSignals(false);
}
