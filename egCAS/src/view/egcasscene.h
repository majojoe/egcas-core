#ifndef EGCASSCENE_H
#define EGCASSCENE_H

#include <QGraphicsScene>
#include <QMargins>

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
         * @brief margins get the margins of a scene
         * @return returns the margins of a scene (left, right, top and bottom)
         */
        QMargins margins(void);
        /**
         * @brief setMargins sets the margins of a scene
         * @param margins the margins must be given as a QMargins object
         */
        void setMargins(QMargins margins);
        /**
         * @brief dimensions get the Height and Width of a sheet
         * @return returns height and widht of a sheet
         */
        QSizeF dimensions(void);
        void setDimensions(QSizeF);
protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect);
private:
        QSizeF m_grid;
        QMargins m_margins;
};

#endif // EGCASSCENE_H
