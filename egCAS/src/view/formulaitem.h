/**
 **
*/
#ifndef FORMULAITEM_H
#define FORMULAITEM_H

#include <QGraphicsItem>
#include <QPainter>

class QwtMathMLDocument;

/**
 * @brief The FormulaItem class implements a QGraphicsItem to be able to use a formula in a QGraphicsView
 */
class FormulaItem : public QGraphicsItem
{
public:        
        ///standard constructor
        explicit FormulaItem(QGraphicsItem *parent = 0);
        ~FormulaItem();
        /**
         * @brief constructor for instantiating a formula with xml representation already available
         * @param formula the mathml representation of the formula to render
         * @param point the left bottom start point for rendering the formula
         * @param parent pointer to parent widget
         */
        explicit FormulaItem(const QString &formula, QPointF point, QGraphicsItem *parent = 0);
        /**
         * @brief override paint from QGraphicsItem
         * @param painter pointer to painter
         * @param option pointer to style options
         * @param widget pointer to parent widget
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        /**
         * @brief setStartPoint set the start point from where to render the formula
         * @param point the starting point (bottom left) point from where to render the formula
         */
        void setStartPoint(QPointF point);
        /**
         * @brief boundingRect must be implemented when inheriting from QGraphicsItem
         * @return the outermost dimensions of the item created
         */
        QRectF boundingRect() const;
        /**
         * @brief setFormulaText sets the text (mathml representation) of the formula
         * @param formula the mathml representation
         */
        void setFormulaText(const QString &formula);
        /**
         * @brief set the generic font size for all formulas (changes the overall font size of all formulas in a document).
         * If the font size of a specific formula should be changed, use the function setFontSize.
         * @param size the font size in points
         */
        static void setBaseFontSize(int size);
        /**
         * @brief set the font size for a formula (changes only the font size of this formula).
         * If the overall font size of all formulas should be changed, use the function setBaseFontSize.
         * @param size the font size in points
         */
        void setFontSize(int size);

protected:
        /**
         * @brief mousePressEvent overrides mousePressEvent from QGraphicsItem
         * @param event pointer to QGraphicsSceneMouseEvent
         */
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        /**
         * @brief mousePressEvent overrides mouseReleaseEvent from QGraphicsItem
         * @param event pointer to QGraphicsSceneMouseEvent
         */
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        /**
         * @brief init initializes a new instance of this class (used in constructor)
         */
        void init(void);

signals:

public slots:
private:
        QString formulaText;
        QScopedPointer<QwtMathMLDocument> mathMlDoc;
        static quint8 s_baseFontSize;
        quint8 fontSize;
        QPointF startPoint;

        Q_DISABLE_COPY(FormulaItem)
};

#endif // FORMULAITEM_H
