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

#include <QVBoxLayout>
#include <QPushButton>
#include "collapsablesectionwidget.h"

CollapsableSectionWidget::CollapsableSectionWidget(CollapsableSectionLayout layout, QWidget* parent) : QWidget(parent)
{
        vLayout = new QVBoxLayout(this);
        vLayout->setSpacing(0);
        vLayout->setContentsMargins(0, 0, 0, 0);
        btn_collapse = new QPushButton(this);
        btn_collapse->setCheckable(true);
        btn_collapse->setChecked(false);
        btn_collapse->setStyleSheet("QPushButton {Text-align:left;padding:3px;background-color: rgb(64, 66, 68);"
                                    "color: rgb(255, 255, 255);border-radius: 1px;}"
                                    "QPushButton:checked{background-color: rgb(100, 100, 100);"
                                    "color: rgb(255, 255, 255);}");
        btn_collapse->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        vLayout->addWidget(btn_collapse);

        sectionWidget = new QWidget(this);
        if (layout == CollapsableSectionLayout::grid) {
                gridLayout = new QGridLayout(sectionWidget);
                gridLayout->setSpacing(3);
                gridLayout->setContentsMargins(0, 3, 0, 3);
                boxLayout = nullptr;
        } else if (layout == CollapsableSectionLayout::vertical) {
                boxLayout = new QVBoxLayout(sectionWidget);
                boxLayout->setSpacing(3);
                boxLayout->setContentsMargins(0, 3, 0, 3);
                gridLayout = nullptr;
        } else {
                boxLayout = new QHBoxLayout(sectionWidget);
                boxLayout->setSpacing(3);
                boxLayout->setContentsMargins(0, 3, 0, 3);
                gridLayout = nullptr;
        }

        vLayout->addWidget(sectionWidget);

        sectionWidget->hide();

        connect(this->btn_collapse, SIGNAL(clicked(bool)), this, SLOT(setVisibility(bool)));
}

CollapsableSectionWidget::~CollapsableSectionWidget()
{

}

void CollapsableSectionWidget::setVisibility(bool visible)
{
        if (visible)
                sectionWidget->show();
        else
                sectionWidget->hide();
}

void CollapsableSectionWidget::setText(const QString & text)
{
        btn_collapse->setText(text);
}

void CollapsableSectionWidget::addWidget(QWidget *widget, int fromRow, int fromColumn, int rowSpan, int columnSpan)
{
        if (!gridLayout)
                return;
        gridLayout->addWidget(widget, fromRow, fromColumn, rowSpan, columnSpan);
}

void CollapsableSectionWidget::addWidget(QWidget *widget, int stretch)
{
        if (!boxLayout)
                return;
        gridLayout->addWidget(widget);
}

void CollapsableSectionWidget::setChecked(bool checked)
{
        btn_collapse->setChecked(checked);
}
