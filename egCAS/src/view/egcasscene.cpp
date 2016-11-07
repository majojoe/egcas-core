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
#include <QGraphicsSceneEvent>
#include <QKeyEvent>
#include <QGraphicsView>
#include "egcasscene.h"
#include "egctextitem.h"
#include "egcpixmapitem.h"
#include "egcformulaitem.h"
#include "egccrossitem.h"
#include "actions/egcactionmapper.h"


EgCasScene::EgCasScene(QObject *parent) :
        QGraphicsScene{parent}, m_cursor{addLine(0,0,0,0,QPen(QColor(Qt::red)))},
        m_nodeUnderline{addLine(0,0,0,0,QPen(QColor(Qt::red)))}
{        
        m_grid = QSizeF(30.0, 30.0);
        m_cursor->setPen(QPen(QBrush(QColor(Qt::red)), 2.0));
        m_nodeUnderline->setPen(QPen(QBrush(QColor(Qt::red)), 2.0));

        m_cross = new (std::nothrow) EgcCrossItem();
        if (m_cross) {
                addItem(m_cross);
        }
}

EgCasScene::~EgCasScene()
{
        m_cursor = nullptr;
        m_nodeUnderline = nullptr;
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
        formulaItem->setPos(point);

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
        item->setPos(point);
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
        item->setPos(point);
        
        return item.take();
}

void EgCasScene::setFormulaCursor(const QLineF& line)
{
        m_cursor->setLine(line);
        m_cursor->show();
}

void EgCasScene::setUnderlineCursor(const QLineF& line)
{
        m_nodeUnderline->setLine(line);
        m_nodeUnderline->show();
}

void EgCasScene::hideFormulaCursors( void )
{
        if (m_cursor)
                m_cursor->hide();
        if (m_nodeUnderline)
                m_nodeUnderline->hide();
}

void EgCasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
        if (!mouseGrabberItem()) {
                if (m_cross) {
                        m_cross->setPos(event->scenePos());
                        m_cross->show();
                        setFocusItem(m_cross);
                }
        }
        QGraphicsScene::mouseReleaseEvent(event);
}

void EgCasScene::itemYieldsFocus(EgcSceneSnapDirection direction, QGraphicsItem& item)
{
        QRectF bounds = item.mapRectToScene(item.boundingRect());

        switch(direction) {
        case EgcSceneSnapDirection::up:
                m_cross->up(bounds.top());
                break;
        case EgcSceneSnapDirection::down:
                m_cross->down(bounds.bottom());
                break;
        case EgcSceneSnapDirection::left:
                m_cross->left(bounds.left());
                break;
        default: //right
                m_cross->right(bounds.right());
                break;
        }

        clearSelection();
        m_cross->show();
        setFocusItem(m_cross);
}

void EgCasScene::triggerFormulaCreation(QPointF point, QKeyEvent* event)
{
        emit createFormula(point, EgcActionMapper::map(event));
}

