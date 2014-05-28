#ifndef EGCASSCENE_H
#define EGCASSCENE_H

#include <QGraphicsScene>

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
protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect);
private:
        QSizeF m_grid;
};

#endif // EGCASSCENE_H
