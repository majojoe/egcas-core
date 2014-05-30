#include <QGraphicsSceneMouseEvent>
#include <libegcas/qwt_mml_document.h>
#include "formulaitem.h"
#include "egcasscene.h"

quint8 FormulaItem::s_baseFontSize = 14;

FormulaItem::FormulaItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
        init();
}

FormulaItem::~FormulaItem()
{

}

FormulaItem::FormulaItem(const QString &formula, QPointF point, QGraphicsItem*parent) :
        QGraphicsItem(parent)
{
        init();
        setFormulaText(formula);
        startPoint = point;
}

void FormulaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        (void) option;
        (void) widget;

        if (!fontSize)
                mathMlDoc->setBaseFontPointSize(static_cast<qreal>(s_baseFontSize));
        else
                mathMlDoc->setBaseFontPointSize(static_cast<qreal>(fontSize));

        QRectF formulaRect(QPointF(0,0), mathMlDoc->size());
        formulaRect.moveBottomLeft(startPoint);
        mathMlDoc->paint( painter, formulaRect.topLeft() );
}

void FormulaItem::setStartPoint(QPointF point)
{
        startPoint = point;
}

QRectF FormulaItem::boundingRect() const
{
        //the start point is the bottom left point of the formula
        QRectF bounds(QPointF(0,0), mathMlDoc->size());
        bounds.moveBottomLeft(startPoint);

        return bounds;
}

void FormulaItem::setFormulaText(const QString &formula)
{
        formulaText = formula;
        mathMlDoc->setContent(formulaText);
}

void FormulaItem::setBaseFontSize(int size)
{
        s_baseFontSize = size;
}

void FormulaItem::setFontSize(int size)
{
        fontSize = size;
}

void FormulaItem::mousePressEvent(QGraphicsSceneMouseEvent*event)
{
        //update();
        QGraphicsItem::mousePressEvent(event);
}

void FormulaItem::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
        //update();
        QGraphicsItem::mouseReleaseEvent(event);
}

void FormulaItem::init()
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
        fontSize = 0;
        mathMlDoc.reset(new QwtMathMLDocument());
}

QVariant FormulaItem::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemPositionChange && scene()) {
         // value is the new position.
         QPointF newPos = value.toPointF();
         EgCasScene* egcasScene = qobject_cast<EgCasScene*>(this->scene());
         if (egcasScene) {
                 QSizeF grid = egcasScene->grid();
                 newPos.setX(qRound(newPos.x()/grid.width()) * grid.width() );
                 newPos.setY(qRound(newPos.y()/grid.height()) * grid.height() );
         }

         return newPos;
     }
     return QGraphicsItem::itemChange(change, value);
 }
