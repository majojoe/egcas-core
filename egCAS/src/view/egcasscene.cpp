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
#include "egcitemtypes.h"


EgCasScene::EgCasScene(EgcAbstractDocument& doc, QObject *parent) :
        m_document{doc}, QGraphicsScene{parent}, m_cursor{addLine(0,0,0,0,QPen(QColor(Qt::red)))},
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

void EgCasScene::drawHorizontalLines(QPainter*painter, const QRectF& rect, qreal leftX, qreal rightX)
{
        qreal x1 = rect.left();
        if (x1 < leftX)
                x1 = leftX;
        qreal y1 = rect.top();
        y1 = qRound(y1/m_grid.height()) * m_grid.height();
        qreal x2 = rect.right();
        if (x2 > rightX)
                x2 = rightX;
        qreal y2 = y1;
        qreal ymax = rect.bottom();
        qreal gridH = m_grid.height();
        QLineF horizontal(x1, y1, x2, y2);

        while (y1 <= ymax) {
                if (m_worksheet.isVisible(QPointF(x1 + .1, y1)))
                        painter->drawLine(horizontal);
                y1 += gridH;
                y2 = y1;
                horizontal.setLine(x1, y1, x2, y2);
        };
}

void EgCasScene::drawVerticalLines(QPainter*painter, const QRectF& rect, qreal leftX, qreal rightX)
{
        quint32 page;
        QRectF area;

        qreal x1 = rect.left();
        if (x1 < leftX)
                x1 = leftX;
        x1 = qRound(x1/m_grid.width()) * m_grid.width();
        qreal y1 = rect.top();
        qreal x2 = x1;
        qreal y2 = rect.bottom();
        qreal xTmp = x1;

        qreal xmax = rect.right();
        if (xmax > rightX)
                xmax = rightX;
        qreal gridW = m_grid.width();
        QLineF vertical(x1, y1, x2, y2);

        page = m_worksheet.pageAtPoint(QPointF(x1, y1));
        area = m_worksheet.activeArea(page);
        y1 = area.top();
        do {
                y2 = qMin(area.bottom(), rect.bottom());
                while (x1 <= xmax) {
                        vertical.setLine(x1, y1, x2, y2);
                        painter->drawLine(vertical);
                        x1 += gridW;
                        x2 = x1;
                };
                x1 = xTmp;
                x2 = xTmp;
                area = m_worksheet.activeArea(++page);
                y1 = area.top();
        } while(y1 < rect.bottom());
}

void EgCasScene::drawActiveAreaBorder(QPainter* painter, const QRectF&rect)
{
        quint32 page = m_worksheet.pageAtPoint(rect.topLeft());
        QRectF area = m_worksheet.activeArea(page);

        painter->save();
        painter->setPen(QPen(Qt::darkGray, 2));

        do {
                painter->drawRect(area);
                area = m_worksheet.activeArea(++page);
        } while (area.top() < rect.bottom());

        painter->restore();
}

void EgCasScene::drawBackground(QPainter*painter, const QRectF&rect)
{
        int nrPages = qFloor((sceneRect().height() + 0.1) / m_worksheet.getSize().height());
        if (nrPages < 1)
                return;

        qreal leftX = m_worksheet.getLeftMargin();
        qreal rightX = m_worksheet.getSize().width() - m_worksheet.getRightMargin();

        painter->save();
        painter->setPen(QPen(Qt::lightGray, 1));

        drawHorizontalLines(painter, rect, leftX, rightX);

        drawVerticalLines(painter, rect, leftX, rightX);

        drawActiveAreaBorder(painter, rect);

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
        } else {
                if (m_cross)
                        m_cross->setPos(event->scenePos());
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

void EgCasScene::triggerFormulaCreation(QPointF point, QKeyEvent *event)
{
        emit createFormula(point, EgcActionMapper::map(event));
}

void EgCasScene::moveItems(bool moveDown, QPointF point)
{
        qreal grid_h = m_grid.height();
        QList<QGraphicsItem *> allItems = items();
        QGraphicsItem* item;

        foreach (item, allItems) {
                if (    item->type() == static_cast<int>(EgcGraphicsItemType::EgcFormulaItemType)
                     || item->type() == static_cast<int>(EgcGraphicsItemType::EgcPixmapItemType)
                     || item->type() == static_cast<int>(EgcGraphicsItemType::EgcTextItemType)) {
                        if (item->pos().y() >= point.y()) {
                                if (moveDown)
                                        item->moveBy(0.0, grid_h);
                                else if (item->pos().y() > point.y())
                                        item->moveBy(0.0, -grid_h);
                        }
                }
        }
}

bool EgCasScene::deleteItem(EgcAbstractFormulaItem* item)
{
        QGraphicsItem* qitem = dynamic_cast<QGraphicsItem*>(item);

        if (!qitem)
                return false;

        return deleteItem(qitem);
}

bool EgCasScene::deleteItem(EgcAbstractPixmapItem* item)
{
        QGraphicsItem* qitem = dynamic_cast<QGraphicsItem*>(item);

        if (!qitem)
                return false;

        return deleteItem(qitem);
}

bool EgCasScene::deleteItem(EgcAbstractTextItem* item)
{
        QGraphicsItem* qitem = dynamic_cast<QGraphicsItem*>(item);

        if (!qitem)
                return false;

        return deleteItem(qitem);
}

bool EgCasScene::deleteItem(QGraphicsItem *item)
{
        if (!item)
                return false;

        itemYieldsFocus(EgcSceneSnapDirection::left, *item);
        removeItem(item);
        delete item;

        return true;
}
