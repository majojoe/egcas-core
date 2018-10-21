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
#include "../document/egcabstractdocument.h"
#include <QXmlStreamWriter>

#if defined( Q_OS_WIN )
QFont EgcTextEntity::s_genericFont = QFont(QString("Times New Roman"), 14);
#else //#if defined( Q_OS_WIN )
QFont EgcTextEntity::s_genericFont = QFont(QString("Century Schoolbook L"), 14);
#endif //#if defined( Q_OS_WIN )


EgcTextEntity::EgcTextEntity(void) : m_item(nullptr), m_font{nullptr}, m_isHtml{false}
{
}

EgcTextEntity::~EgcTextEntity()
{
        delete m_font;
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

        if (m_isHtml)
                return m_item->getHtmlText();
        else
                return m_item->getText();
}

void EgcTextEntity::setItem(EgcAbstractTextItem* item)
{
        m_item = item;
}

EgcAbstractTextItem*EgcTextEntity::getItem()
{
        return m_item;
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

        m_isHtml = false;
        m_item->setText(text);
}

void EgcTextEntity::setHtmlText(QString text)
{
        if (!m_item)
                return;

        m_isHtml = true;
        m_item->setHtmlText(text);
}

QFont EgcTextEntity::getFont(void) const
{
        if (!m_font)
                return s_genericFont;
        else
                return *m_font;
}

void EgcTextEntity::setFont(const QFont& font)
{
        if (m_font)
                delete m_font;
        if (m_isHtml)   // will not be set if text is html type
                return;
        m_font = new QFont(font);
        //delete font for this entity if it is the same as the generic font
        if (*m_font == s_genericFont) {
                delete m_font;
                m_font = nullptr;
        }
}

QFont& EgcTextEntity::getEntityFont()
{
        if (m_font)
                return *m_font;
        else
                return s_genericFont;
}

void EgcTextEntity::setGenericFont(const QFont& font)
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

void EgcTextEntity::setEditMode()
{
        if (m_item)
                m_item->setEditMode();
}

void EgcTextEntity::serialize(QXmlStreamWriter& stream, SerializerProperties &properties)
{
        stream.writeStartElement("text_entity");
        if (m_font && !m_isHtml)
                stream.writeAttribute("font", m_font->toString());
        if (m_isHtml)
                stream.writeAttribute("is_html", "true");
        stream.writeAttribute("pos_x", QString("%1").arg(getPosition().x()));
        stream.writeAttribute("pos_y", QString("%1").arg(getPosition().y()));
        stream.writeCharacters(getText());
        stream.writeEndElement(); // document
}

void EgcTextEntity::deserialize(QXmlStreamReader& stream, SerializerProperties& properties)
{
        (void) properties;

        if (stream.name() == QLatin1String("text_entity")) {
                QXmlStreamAttributes attr = stream.attributes();
                if (attr.hasAttribute("font")) {
                        QString font_str = attr.value("font").toString();
                        QFont fnt;
                        fnt.fromString(font_str);
                        setFont(fnt);
                }
                if (attr.hasAttribute("is_html")) {
                        QString is_html_str = attr.value("is_html").toString();
                        if (is_html_str == "true")
                                setIsHtml();
                }
                if (attr.hasAttribute("pos_x") && attr.hasAttribute("pos_y")) {
                        qreal x = attr.value("pos_x").toFloat();
                        qreal y = attr.value("pos_y").toFloat();
                        setPosition(QPointF(x, y));
                }
                setText(stream.readElementText());
        }
        if (!stream.isEndElement())
                stream.skipCurrentElement();
}

bool EgcTextEntity::isHtmlText()
{
        return m_isHtml;
}

void EgcTextEntity::setIsHtml()
{
        m_isHtml = true;
}

