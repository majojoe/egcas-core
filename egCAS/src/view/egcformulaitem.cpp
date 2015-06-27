/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#include <QGraphicsSceneMouseEvent>
#include <libegcas/eg_mml_document.h>
#include "egcformulaitem.h"
#include "egcasscene.h"

quint8 EgcFormulaItem::s_baseFontSize = 14;

EgcFormulaItem::EgcFormulaItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
        init();
}

EgcFormulaItem::~EgcFormulaItem()
{

}

EgcFormulaItem::EgcFormulaItem(const QString &formula, QPointF point, int size, QGraphicsItem *parent) :
        QGraphicsItem(parent)
{
        init();
        setFormulaText(formula);
        startPoint = point;
        setFontSize(size);
}

EgcFormulaItem::EgcFormulaItem(const QString &formula, int size, QGraphicsItem *parent) :
        QGraphicsItem(parent)
{
        init();
        setFormulaText(formula);
        setFontSize(size);
        startPoint = QPointF(0.0, boundingRect().height());
}

void EgcFormulaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        (void) option;
        (void) widget;

        if (!fontSize)
                mathMlDoc->setBaseFontPixelSize(static_cast<qreal>(s_baseFontSize));
        else
                mathMlDoc->setBaseFontPixelSize(static_cast<qreal>(fontSize));

        QRectF formulaRect(QPointF(0,0), mathMlDoc->size());
        formulaRect.moveBottomLeft(startPoint);
        mathMlDoc->paint( painter, formulaRect.topLeft() );

        if (isSelected()) {
                painter->save();
                painter->setPen( QPen(Qt::black,
                                      qreal( 1 ),
                                      Qt::PenStyle( Qt::DashLine ) ) );
                painter->drawRect( formulaRect );
                painter->restore();
        }
}

void EgcFormulaItem::setStartPoint(QPointF point)
{
        startPoint = point;
}

QRectF EgcFormulaItem::boundingRect() const
{
        //the start point is the bottom left point of the formula
        QRectF bounds(QPointF(0,0), mathMlDoc->size());
        bounds.moveBottomLeft(startPoint);

        return bounds.adjusted(0, -1.0, +1.0, +1.0);
}

void EgcFormulaItem::setFormulaText(const QString &formula)
{
        formulaText = formula;
        mathMlDoc->setContent(formulaText);
}

void EgcFormulaItem::setBaseFontSize(int size)
{
        s_baseFontSize = size;
}

void EgcFormulaItem::setFontSize(int size)
{
        fontSize = size;
        mathMlDoc->setBaseFontPixelSize(size);
        //mathMlDoc->
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
}

void EgcFormulaItem::init()
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
        fontSize = 0;
        mathMlDoc.reset(new EgMathMLDocument());
        mathMlDoc->setBaseFontPixelSize(s_baseFontSize);
}

QVariant EgcFormulaItem::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemPositionChange && scene()) {
         // value is the new position.
         QPointF newPos = value.toPointF();
         QSizeF grid = qobject_cast<EgCasScene*>(this->scene())->grid();
         newPos.setX(qRound(newPos.x()/grid.width()) * grid.width() );
         newPos.setY(qRound(newPos.y()/grid.height()) * grid.height() );

         return newPos;
     }
     return QGraphicsItem::itemChange(change, value);
 }

QPointF EgcFormulaItem::getPos( void ) const
{
        return pos();
}
