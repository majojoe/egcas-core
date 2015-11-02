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

#include <QGraphicsSceneMouseEvent>
#include <libegcas/eg_mml_document.h>
#include "egcformulaitem.h"
#include "egcasscene.h"
#include "entities/egcabstractformulaentity.h"
#include "egcabstractformulaitem.h"
#include "egcscreenpos.h"
#include "actions/egcaction.h"

quint8 EgcFormulaItem::s_baseFontSize = 14;

EgcFormulaItem::EgcFormulaItem(QGraphicsItem *parent) :
    QGraphicsItem{parent}, m_fontSize{0}, m_posChanged{false}, m_entity{nullptr}
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
        m_mathMlDoc.reset(new EgMathMLDocument());
        m_mathMlDoc->setBaseFontPixelSize(s_baseFontSize);
        m_screenPos.reset(new EgcScreenPos());
}

EgcFormulaItem::~EgcFormulaItem()
{

}

EgcFormulaItem::EgcFormulaItem(const QString &formula, QPointF point, int size, QGraphicsItem *parent) :
        EgcFormulaItem{parent}
{
        setFormulaText(formula);
        QGraphicsItem::setPos(point);
        setFontSize(size);
}

EgcFormulaItem::EgcFormulaItem(const QPointF point, QGraphicsItem *parent) :
        EgcFormulaItem{parent}
{
        QGraphicsItem::setPos(point);
}

void EgcFormulaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        (void) option;
        (void) widget;

        if (!m_fontSize)
                m_mathMlDoc->setBaseFontPixelSize(static_cast<qreal>(s_baseFontSize));
        else
                m_mathMlDoc->setBaseFontPixelSize(static_cast<qreal>(m_fontSize));

        QRectF formulaRect(QPointF(0,0), m_mathMlDoc->size());
        m_mathMlDoc->paint( painter, formulaRect.topLeft() );
        m_screenPos->setPositions(m_mathMlDoc->getRenderingPositions());

        if (isSelected()) {
#ifdef DEBUG_SCENE_RENDERING_POS
                QVector<EgRenderingPosition> renderingPosition = m_mathMlDoc->getRenderingPositions();

                EgRenderingPosition position;
                Qt::GlobalColor color = Qt::gray;
                foreach (position, renderingPosition) {
                        if (position.m_index == 0) {
                                painter->setPen(Qt::darkGray);
                                painter->drawRect(position.m_itemRect);
                        } else {
                                painter->setPen(color);
                                painter->drawRect(position.m_itemRect);
                        }

                        if (color == Qt::darkYellow)
                                color = Qt::gray;
                        else
                                color = static_cast<Qt::GlobalColor>(static_cast<int>(color) + 1);
                }
#endif //#ifdef DEBUG_SCENE_RENDERING_POS

                painter->save();
                painter->setPen( QPen(Qt::black,
                                      qreal( 1 ),
                                      Qt::PenStyle( Qt::DashLine ) ) );
                painter->drawRect( formulaRect );
                painter->restore();
        }
}

QRectF EgcFormulaItem::boundingRect() const
{
        //the start point is the bottom left point of the formula
        QRectF bounds(QPointF(0,0), m_mathMlDoc->size());

        return bounds.adjusted(0, -1.0, +1.0, +1.0);
}

void EgcFormulaItem::setFormulaText(const QString &formula)
{
        m_mathMlDoc->setContent(formula);
}

void EgcFormulaItem::setBaseFontSize(int size)
{
        s_baseFontSize = size;
}

void EgcFormulaItem::setFontSize(int size)
{
        m_fontSize = size;
        m_mathMlDoc->setBaseFontPixelSize(size);
}

int EgcFormulaItem::getFontSize(void)
{
        return m_fontSize;
}

void EgcFormulaItem::mousePressEvent(QGraphicsSceneMouseEvent*event)
{
        //update();
        QGraphicsItem::mousePressEvent(event);
}

void EgcFormulaItem::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
        //update();
        QGraphicsItem::mouseReleaseEvent(event);
        if (m_posChanged) {
                m_posChanged = false;
                //signal entity that item position has changed
                if (m_entity)
                        m_entity->itemChanged(EgcItemChangeType::posChanged);
        }

}

QVariant EgcFormulaItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
     if (change == ItemPositionChange && scene()) {
             m_posChanged = true;
             // value is the new position.
             return snapGrid(value.toPointF());
     }
     
     return QGraphicsItem::itemChange(change, value);
}

void EgcFormulaItem::setEntity(EgcAbstractFormulaEntity* entity)
{
        m_entity = entity;
}

QPointF EgcFormulaItem::getPosition( void ) const
{
        return pos();
}

void EgcFormulaItem::setPos(const QPointF &pos)
{
        QGraphicsItem::setPos(snapGrid(pos));
}

void EgcFormulaItem::setGenericFontSize(int size)
{
        setBaseFontSize(size);
}

int EgcFormulaItem::getGenericFontSize(void)
{
        return s_baseFontSize;
}

void EgcFormulaItem::updateView(void)
{
        if (!m_entity)
                return;

        prepareGeometryChange();
        m_mathMlDoc->setContent(m_entity->getMathMlCode());
        update();
}

QSizeF EgcFormulaItem::getGrid(void)
{
        QSizeF grid;

        QGraphicsScene *scene = this->scene();
        if (scene) {
                grid = static_cast<EgCasScene*>(scene)->grid();
        }

        return grid;
}

const EgcScreenPos& EgcFormulaItem::getScreenPos(void) const
{
        return *m_screenPos.data();
}

void EgcFormulaItem::focusInEvent(QFocusEvent * event)
{
        (void) event;
        if (!m_entity)
                return;
        EgcAction action;
        action.m_op = EgcOperations::formulaActivated;
        m_entity->handleAction(action);
}

void EgcFormulaItem::focusOutEvent(QFocusEvent * event)
{
        (void) event;
        if (!m_entity)
                return;
        EgcAction action;
        action.m_op = EgcOperations::formulaDeactivated;
        m_entity->handleAction(action);
}

void EgcFormulaItem::showUnderline(quint32 mathmlId)
{
        EgRenderingPosition renderPos;
        renderPos = m_screenPos->findRenderingData(mathmlId, 0);
        if (renderPos.m_nodeId) {
                EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
                if (scn)
                        scn->setUnderlineCursor(QLineF(renderPos.m_itemRect.bottomLeft(), renderPos.m_itemRect.bottomRight()));
        }
}

void EgcFormulaItem::showLeftCursor(quint32 mathmlId, quint32 subindex)
{
        EgRenderingPosition renderPos;
        renderPos = m_screenPos->findRenderingData(mathmlId, subindex);
        if (renderPos.m_nodeId) {
                EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
                if (scn)
                        scn->setUnderlineCursor(QLineF(renderPos.m_itemRect.topLeft(), renderPos.m_itemRect.bottomLeft()));
        }
}

void EgcFormulaItem::showRightCursor(quint32 mathmlId, quint32 subindex)
{
        EgRenderingPosition renderPos;
        renderPos = m_screenPos->findRenderingData(mathmlId, subindex);
        if (renderPos.m_nodeId) {
                EgCasScene* scn = qobject_cast<EgCasScene*>(scene());
                if (scn)
                        scn->setUnderlineCursor(QLineF(renderPos.m_itemRect.topRight(), renderPos.m_itemRect.bottomRight()));
        }
}
