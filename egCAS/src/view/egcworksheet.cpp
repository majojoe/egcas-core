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
#include <QtMath>
#include "egcworksheet.h"

EgcWorksheet::EgcWorksheet()
{

}

QSizeF EgcWorksheet::getSize(void)
{
        return m_size;
}

void EgcWorksheet::setSize(QSizeF size)
{
        m_size = size;
}

qreal EgcWorksheet::getLeftMargin(void)
{
        return m_leftMargin;
}

void EgcWorksheet::setLeftMargin(qreal margin)
{
        m_leftMargin = margin;
}

qreal EgcWorksheet::getRightMargin(void)
{
        return m_rightMargin;
}

void EgcWorksheet::setRightMargin(qreal margin)
{
        m_rightMargin = margin;
}

qreal EgcWorksheet::getTopMargin(void)
{
        return m_topMargin;
}

void EgcWorksheet::setTopMargin(qreal margin)
{
        m_topMargin = margin;
}

qreal EgcWorksheet::getBottomMargin(void)
{
        return m_bottomMargin;
}

void EgcWorksheet::setBottomMargin(qreal margin)
{
        m_bottomMargin = margin;
}

QPointF EgcWorksheet::snapWorksheet(const QPointF& point)
{
        qreal x = point.x();
        qreal y = point.y();

        qreal page = qFloor(y / m_size.height());
        qreal yOffset = y - (page * m_size.height());

        if (x < m_leftMargin)
                x = m_leftMargin;
        if (x > (m_size.width() - m_rightMargin))
                x = (m_size.width() - m_rightMargin);
        if (yOffset < m_topMargin)
                yOffset = m_topMargin;
        if (yOffset > (m_size.height() - m_bottomMargin))
                yOffset = m_size.height() - m_bottomMargin;

        return QPoint(x, yOffset + (page * m_size.height()));
}

QPointF EgcWorksheet::snapWorksheet(const QRectF& item)
{
        QPointF newTopLeftPos = item.topLeft();
        qreal page = qFloor(item.top() / m_size.height());
        qreal topBorder = (page * m_size.height()) + m_topMargin;
        qreal bottomBorder = (page * m_size.height()) + m_size.height() - m_bottomMargin;
        qreal rightBorder = m_size.width() - m_rightMargin;

        if (item.right() > rightBorder) {
                QRectF tmp(item);
                tmp.moveRight(rightBorder);
                newTopLeftPos = tmp.topLeft();
        }
        if (item.left() < m_leftMargin) {
                QRectF tmp(item);
                tmp.moveLeft(m_leftMargin);
                newTopLeftPos = tmp.topLeft();
        }
        if (item.bottom() > bottomBorder) {
                QRectF tmp(item);
                tmp.moveBottom(bottomBorder);
                newTopLeftPos = tmp.topLeft();
        }
        if (item.top() < topBorder) {
                QRectF tmp(item);
                tmp.moveTop(topBorder);
                newTopLeftPos = tmp.topLeft();
        }

        return newTopLeftPos;
}

bool EgcWorksheet::itemWrapsToNewPage(const QRectF& item, qreal yOffset)
{
        if (yOffset < 0)
                return false;

        qreal x = item.topLeft().x();
        qreal y = item.topLeft().y();

        qreal page = qFloor(y / m_size.height());
        qreal yPageOffset = y - (page * m_size.height());

        if (yPageOffset + yOffset + m_bottomMargin > m_size.height())
                return true;

        return false;
}

bool EgcWorksheet::itemOverlapsPageWidth(const QRectF& item, qreal& overlapsPageBy)
{
        bool retval = false;
        overlapsPageBy = item.topRight().x() - m_size.width();

        if (overlapsPageBy < 0) {
                overlapsPageBy = 0;
                retval = true;
        }

        return retval;
}

