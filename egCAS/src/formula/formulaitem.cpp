#include <libegcas/qwt_mml_document.h>
#include "formulaitem.h"

quint8 FormulaItem::s_baseFontSize = 10;

FormulaItem::FormulaItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
        init();
}

FormulaItem::FormulaItem(const QString &formula, QGraphicsItem*parent) :
        QGraphicsItem(parent)
{
        init();
        setFormulaText(formula);
}

void FormulaItem::paint(QPainter*painter, const QStyleOptionGraphicsItem*option, QWidget*widget)
{
//        QPainter painter( this );
//        painter.setClipRegion( event->region() );

//        painter.fillRect( event->rect(), Qt::white );

//        renderFormula( &painter );
//    }

//    void FormulaView::renderFormula( QPainter *painter ) const
//    {
//        QwtMathMLDocument doc;
//        doc.setContent( d_formula );
//        if ( d_colors )
//        {
//            doc.setBackgroundColor( Qt::darkCyan );
//            doc.setForegroundColor( Qt::yellow );
//        }
//        else
//        {
//            doc.setBackgroundColor( Qt::white );
//            doc.setForegroundColor( Qt::black );
//        }
//        doc.setBaseFontPointSize( d_fontSize );
//    #if MML_TEST
//        doc.setDrawFrames( d_drawFrames );
//    #endif

//        QRectF docRect;
//        docRect.setSize( doc.size() );
//        docRect.moveCenter( rect().center() );

//        if ( d_transformation )
//        {
//            const double scaleF = d_scale ? 2.0 : 1.0;

//            painter->save();

//            painter->translate( docRect.center() );
//            painter->rotate( d_rotation );
//            painter->scale( scaleF, scaleF );
//            painter->translate( docRect.topLeft() - docRect.center() );
//            doc.paint( painter, QPointF( 0.0, 0.0 ) );

//            painter->restore();
//        }
//        else
//        {
//            doc.paint( painter, docRect.topLeft() );
//        }

}

QRectF FormulaItem::boundingRect() const
{
        //return outermost shape of formula
        //return QRectF();
}

void FormulaItem::setFormulaText(const QString &formula)
{
        formulaText = formula;
        mathMlDoc.setContent(formulaText);
}

void FormulaItem::setBaseFontSize(quint8 size)
{
        s_baseFontSize = size;
}

void FormulaItem::setFontSize(quint8 size)
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
        setFlag(ItemIsMovable);
}
