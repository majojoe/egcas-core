/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#include <QFont>
#include "egctextentity.h"
#include "egctextitem.h"

QFont EgcTextEntity::s_genericFont = QFont();

EgcTextEntity::EgcTextEntity(void) : m_item(nullptr)
{
}

EgcTextEntity::~EgcTextEntity()
{
}

EgcEntityType EgcTextEntity::getEntityType(void) const
{
        return EgcEntityType::Text;
}

QPointF EgcTextEntity::getPosition(void) const
{
        if (m_item)
                return m_item->getPosition();
        else
                return QPointF(0.0,0.0);
}

QString EgcTextEntity::getText(void) const
{
        if (!m_item)
                return "";

        m_item->getText();
}

QFont EgcTextEntity::getFont(void) const
{
        if (!m_item)
                return s_genericFont;

        return m_item->getFont();
}

void EgcTextEntity::setItem(EgcAbstractTextItem* item)
{
        m_item = item;
}

void EgcTextEntity::setPosition(QPointF pos)
{
        if (!m_item)
                return;

        m_item->setPos(pos);
}

void EgcTextEntity::setText(QString text)
{
        if (!m_item)
                return;

        m_item->setText(text);
}

void EgcTextEntity::setFont(const QFont& font)
{
        if (!m_item)
                return;

        m_item->setFont(font);
}

void EgcTextEntity::setGenericFont(QFont& font)
{
        s_genericFont = font;
}

QFont& EgcTextEntity::getGenericFont(void)
{
        return s_genericFont;
}

void EgcTextEntity::itemChanged(EgcItemChangeType changeType)
{

}
