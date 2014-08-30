#ifndef EGCASSCENE_H
#define EGCASSCENE_H

#include <QGraphicsScene>

class FormulaItem;

class EgCasScene : public QGraphicsScene
{
        Q_OBJECT
public:
        ///constructor of EgCasScene
        explicit EgCasScene(QObject *parent = 0);
        /**
         * @brief grid return the grid size
         * @return the grid size as QSizeF
         */
        QSizeF grid(void);
        /**
         * @brief setGrid set the grid size to be used as background
         * @param grid the grid size to be set
         */
        void setGrid(QSizeF grid);
        /**
         * @brief addText add text to the scene (overrides the standard function)
         * @param text the text to add
         * @param font the font to use
         * @return a pointer to the text item
         */
        QGraphicsTextItem * addText(const QString & text, const QFont & font = QFont());
        /**
         * @brief addPixmap add a pixmap to the scene (overrides the standard function and adds a egcaspixmapitem)
         * @param pixmap the pixmap to add to the scene
         * @return a pointer to the pixmap item
         */
        QGraphicsPixmapItem * addPixmap(const QPixmap & pixmap);
        /**
         * @brief addFormula add a formula to the graphicsscene
         * @param formula the formula to be rendered
         * @param size the font size in pixel of the forumla
         * @return a pointer to the formula added
         */
        FormulaItem * addFormula(const QString & formula, int size);
protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect);
private:
        QSizeF m_grid;
};

#endif // EGCASSCENE_H
