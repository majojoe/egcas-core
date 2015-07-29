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

#include <QGraphicsScene>
#include <QtCore>
#include <QPainter>
#include "egcasscene.h"
#include "egctextitem.h"
#include "egcpixmapitem.h"
#include "egcformulaitem.h"


EgCasScene::EgCasScene(QObject *parent) :
        QGraphicsScene(parent)
{        
        m_grid = QSizeF(30.0, 30.0);
}

QSizeF EgCasScene::grid()
{
        return m_grid;
}

void EgCasScene::setGrid(QSizeF grid)
{
        m_grid = grid;
}

void EgCasScene::drawBackground(QPainter*painter, const QRectF&rect)
{
        //draw horizontal lines
        painter->save();
        painter->setPen(QPen(Qt::lightGray, 1));
        qreal x1 = rect.left();
        qreal y1 = rect.top();
        y1 = qRound(y1/m_grid.height()) * m_grid.height();
        qreal x2 = rect.right();
        qreal y2 = rect.top();
        y2 = qRound(y2/m_grid.height()) * m_grid.height();
        qreal ymax = rect.bottom();
        qreal gridH = m_grid.height();
        QLineF horizontal(x1, y1, x2, y2);

        while (y1 <= ymax) {
                painter->drawLine(horizontal);
                y1 += gridH;
                y2 += gridH;
                horizontal.setLine(x1, y1, x2, y2);
        };

        x1 = rect.left();
        x1 = qRound(x1/m_grid.width()) * m_grid.width();
        y1 = rect.top();
        x2 = rect.left();
        x2 = qRound(x2/m_grid.width()) * m_grid.width();
        y2 = rect.bottom();

        qreal xmax = rect.right();
        qreal gridW = m_grid.width();
        QLineF vertical(x1, y1, x2, y2);

        while (x1 <= xmax) {
                painter->drawLine(vertical);
                x1 += gridW;
                x2 += gridW;
                vertical.setLine(x1, y1, x2, y2);
        };

        painter->restore();
}

QGraphicsTextItem * EgCasScene::addText(const QString & text, const QFont & font)
{
        EgcTextItem *textItem = new (std::nothrow) EgcTextItem(text);
        if (textItem) {
                textItem->setFont(font);
                addItem(textItem);
        }

        return static_cast<QGraphicsTextItem*>(textItem);
}

QGraphicsPixmapItem * EgCasScene::addPixmap(const QPixmap & pixmap)
{
        EgcPixmapItem *pixmapItem = new (std::nothrow) EgcPixmapItem(pixmap);
        if (pixmapItem)
                addItem(pixmapItem);

        return static_cast<QGraphicsPixmapItem*>(pixmapItem);
}


EgcFormulaItem * EgCasScene::addFormula(const QString & formula, int size, QPointF point)
{
        EgcFormulaItem *formulaItem = new (std::nothrow) EgcFormulaItem(formula, point, size);
        if (formulaItem)
                addItem(formulaItem);

        return formulaItem;
}

EgcFormulaItem* EgCasScene::addFormula(EgcAbstractFormulaEntity& formula, QPointF point)
{
        QScopedPointer<EgcFormulaItem> formulaItem(new EgcFormulaItem(point));
        if (formulaItem.isNull())
                return nullptr;

        formulaItem->setEntity(&formula);
        formula.setItem(formulaItem.data());

        addItem(formulaItem.data());

        return formulaItem.take();
}

EgcTextItem* EgCasScene::addText(EgcAbstractTextEntity& text, QPointF point)
{
        QScopedPointer<EgcTextItem> item(new EgcTextItem(point));
        if (item.isNull())
                return nullptr;

        item->setEntity(&text);
        text.setItem(item.data());

        addItem(item.data());
        item->setFont(text.getGenericFont());

        return item.take();
}

EgcPixmapItem* EgCasScene::addPixmap(EgcAbstractPixmapEntity& pixmap, QPointF point)
{
        QScopedPointer<EgcPixmapItem> item(new EgcPixmapItem(point));
        if (item.isNull())
                return nullptr;

        item->setEntity(&pixmap);
        pixmap.setItem(item.data());

        addItem(item.data());

        return item.take();
}
