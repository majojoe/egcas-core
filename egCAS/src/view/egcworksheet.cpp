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
#include <egcasscene.h>
#include "egcworksheet.h"

EgcWorksheet::EgcWorksheet(EgCasScene& scene) : m_size{QSize(2100,2900)}, m_leftMargin{100}, m_rightMargin{100},
                                                m_topMargin{100}, m_bottomMargin{100}, m_scene{scene}
{

}

QSizeF EgcWorksheet::getSize(void)
{
        return m_size;
}

void EgcWorksheet::setSize(QSizeF size)
{
        m_size = size;
        QRectF scnRect = m_scene.sceneRect();

        m_scene.setSceneRect(scnRect.x(), scnRect.y(), m_size.width(), m_size.height() * nrPages());

        //set grid to be a multiple of size height (is adjusted by setGrid)
        m_scene.setGrid(m_scene.grid().grid());
}

qreal EgcWorksheet::getLeftMargin(void) const
{
        return m_leftMargin;
}

void EgcWorksheet::setLeftMargin(qreal margin)
{
        m_leftMargin = margin;
}

qreal EgcWorksheet::getRightMargin(void) const
{
        return m_rightMargin;
}

void EgcWorksheet::setRightMargin(qreal margin)
{
        m_rightMargin = margin;
}

qreal EgcWorksheet::getTopMargin(void) const
{
        return m_topMargin;
}

void EgcWorksheet::setTopMargin(qreal margin)
{
        m_topMargin = margin;
}

qreal EgcWorksheet::getBottomMargin(void) const
{
        return m_bottomMargin;
}

void EgcWorksheet::setBottomMargin(qreal margin)
{
        m_bottomMargin = margin;
}

QPointF EgcWorksheet::snapWorksheet(const QPointF& point) const
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

QPointF EgcWorksheet::snapWorksheet(const QRectF& item) const
{
        QPointF newTopLeftPos = item.topLeft();
        qreal page = qFloor(item.top() / m_size.height());
        qreal topBorder = (page * m_size.height()) + m_topMargin;
        qreal bottomBorder = (page * m_size.height()) + m_size.height() - m_bottomMargin;
        qreal rightBorder = m_size.width() - m_rightMargin;

        qreal nx = newTopLeftPos.x();
        qreal ny = newTopLeftPos.y();

        if (item.right() > rightBorder) {
                QRectF tmp(item);
                tmp.moveRight(rightBorder);
                newTopLeftPos = tmp.topLeft();
                nx = newTopLeftPos.x();
        }
        if (item.left() < m_leftMargin) {
                QRectF tmp(item);
                tmp.moveLeft(m_leftMargin);
                newTopLeftPos = tmp.topLeft();
                nx = newTopLeftPos.x();
        }
        if (item.bottom() > bottomBorder) {
                QRectF tmp(item);
                tmp.moveBottom(bottomBorder);
                newTopLeftPos = tmp.topLeft();
                ny = newTopLeftPos.y();
        }
        if (item.top() < topBorder) {
                QRectF tmp(item);
                tmp.moveTop(topBorder);
                newTopLeftPos = tmp.topLeft();
                ny = newTopLeftPos.y();
        }

        return QPointF(nx, ny);
}

bool EgcWorksheet::itemWrapsToNewPage(const QRectF& item, qreal yOffset)
{
        qreal x = item.x();
        qreal y = item.y();

        qreal page = qFloor(y / m_size.height());
        qreal yPageOffset = y - (page * m_size.height());

        if (yOffset > 0) {
                if ((yPageOffset + yOffset + m_bottomMargin + item.height()) > m_size.height())
                        return true;
        } else {
                if ((yPageOffset - (-yOffset + m_topMargin)) < 0)
                        return true;
        }

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

bool EgcWorksheet::isVisible(QPointF point) const
{
        bool retval = true;

        qreal page = qFloor(point.y() / m_size.height());
        qreal yOffset = point.y() - (page * m_size.height());

        if (yOffset < m_topMargin)
                retval = false;
        if (yOffset > (m_size.height() - m_bottomMargin))
                retval = false;
        if (point.x() < m_leftMargin)
                retval = false;
        if (point.x() > (m_size.width() - m_rightMargin))
                retval = false;

        return retval;
}

quint32 EgcWorksheet::pageAtPoint(QPointF point) const
{
        return qFloor(point.y() / m_size.height());
}

QRectF EgcWorksheet::activeArea(quint32 pageIndex) const
{
        QRectF activeRect(m_leftMargin, m_topMargin, m_size.width() - (m_leftMargin + m_rightMargin),
                    m_size.height() - (m_topMargin + m_bottomMargin));
        activeRect.moveTop((pageIndex * m_size.height()) + m_topMargin);
        return activeRect;
}

quint32 EgcWorksheet::nrPages(void) const
{
        return qFloor((m_scene.sceneRect().height() + 0.1) / m_size.height());
}

bool EgcWorksheet::onLastPage(QPointF point) const
{
        if (pageAtPoint(point) + 1 == nrPages())
                return true;

        return false;
}
