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
#include <QSignalMapper>
#include "algebrasection.h"

QVector<MathElement> AlgebraSection::s_buttonVect = AlgebraSection::initCommandVector();

AlgebraSection::AlgebraSection(QWidget *parent) : QWidget(parent), m_nrCoulumns{6}
{
        m_section = new CollapsableSectionWidget(CollapsableSectionWidget::CollapsableSectionLayout::grid, this);
        m_signalMapper = new QSignalMapper(this);

        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setSpacing(0);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->addWidget(m_section);

        MathElement element;
        QPushButton *btn;
        quint32 index = 0;
        foreach (element, s_buttonVect) {
                btn = new QPushButton(m_section);
                btn->setStyleSheet("padding:0px;background-color:rgb(64, 66, 68);");
                btn->setFlat(true);
                if (element.m_isIcon)
                        btn->setIcon(QIcon(element.m_designator));
                else
                        btn->setText(element.m_designator);
                btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
//                btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
//                btn->setMaximumWidth(40);
                connect(btn, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
                m_signalMapper->setMapping(btn, element.m_command);
                m_section->addWidget(btn, index / m_nrCoulumns, index % m_nrCoulumns, 1, 1);
                index++;
        }

        connect(m_signalMapper, SIGNAL(mapped(QString)), this, SIGNAL(clicked(QString)));
}

QVector<MathElement> AlgebraSection::initCommandVector(void)
{

        QVector<MathElement> vect;

        vect.append(MathElement("+", "+"));
        vect.append(MathElement("-", "-"));
        vect.append(MathElement("/", "/"));
        vect.append(MathElement("*", "*"));
        vect.append(MathElement("( )", "("));
        vect.append(MathElement("( )", "("));
        vect.append(MathElement("( )", "("));

        return vect;
}

void AlgebraSection::clicked(QString cmd)
{

}

void AlgebraSection::setText(const QString & text)
{
        m_section->setText(text);
}

void AlgebraSection::setChecked(void)
{
        m_section->setChecked(true);
}
