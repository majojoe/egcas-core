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
