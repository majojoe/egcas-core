/*
Copyright (c) 2014 - 2018, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
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
#include <richtextedit/mrichtextedit.h>
#include "richtexteditor.h"
#include <QApplication>
#include <QDialog>
#include <QPushButton>

RichTextEditor::RichTextEditor(QWidget* parent) : QObject(parent)
{
        m_dialog = new QDialog(0);
        m_rte = new MRichTextEdit(m_dialog, false);
        m_gl = new QGridLayout(m_dialog);
        m_ok_btn = new QPushButton(m_dialog);
        m_ok_btn->setText(QObject::tr("Ok"));
        m_cancel_btn = new QPushButton(m_dialog);
        m_cancel_btn->setText(QObject::tr("Cancel"));
        m_gl->addWidget(m_rte,0,0,1,2);
        m_gl->addWidget(m_cancel_btn, 1, 0);
        m_gl->addWidget(m_ok_btn, 1, 1);
        m_dialog->setWindowTitle(QObject::tr("Rich text editor"));
        m_dialog->setMinimumWidth (400);
        m_dialog->setMinimumHeight(400);
        m_dialog->setModal(true);
        connect(m_ok_btn, &QPushButton::clicked, this, &RichTextEditor::ok_clicked);
        connect(m_cancel_btn, &QPushButton::clicked, this, &RichTextEditor::cancel_clicked);
}

RichTextEditor::~RichTextEditor()
{
        delete m_dialog;
        m_dialog = nullptr;
}

QString RichTextEditor::exec(QString initialText)
{
        m_initialText = initialText;
        m_rte->setText(initialText);
        m_dialog->exec();

        return m_rte->toHtml();
}

void RichTextEditor::ok_clicked()
{
        m_dialog->accept();
}

void RichTextEditor::cancel_clicked()
{
        m_rte->setText(m_initialText);
        m_dialog->reject();
}

