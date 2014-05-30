#include <QGraphicsScene>
#include <QtCore>
#include <QPainter>
#include "egcasscene.h"


EgCasScene::EgCasScene(QObject *parent) :
        QGraphicsScene(parent)
{        
        m_grid = QSizeF(30.0, 30.0);
}

QSizeF EgCasScene::grid()
{
        return m_grid;
}

void EgCasScene::setGrid(QSizeF grid)
{
        m_grid = grid;
}

void EgCasScene::drawBackground(QPainter*painter, const QRectF&rect)
{
        //draw horizontal lines
        painter->save();
        painter->setPen(QPen(Qt::lightGray, 1));
        qreal gridH = m_grid.height();
        qreal gridW = m_grid.width();
        qreal x1 = rect.left();
        qreal y1 = rect.top();
        y1 = qRound(y1/gridH) * gridH;
        qreal x2 = rect.right();
        qreal y2 = rect.top();
        y2 = qRound(y2/gridH) * gridH;
        qreal ymax = rect.bottom();
        QLineF horizontal(x1, y1, x2, y2);

        while (y1 <= ymax) {
                if (x1 < gridW)
                        horizontal.setLine(gridW, y1, x2, y2);
                if (y1 >= gridH)
                        painter->drawLine(horizontal);
                y1 += gridH;
                y2 += gridH;
                horizontal.setLine(x1, y1, x2, y2);
        };

        x1 = rect.left();
        x1 = qRound(x1/gridW) * gridW;
        y1 = rect.top();
        x2 = rect.left();
        x2 = qRound(x2/gridW) * gridW;
        y2 = rect.bottom();

        qreal xmax = rect.right();        
        QLineF vertical(x1, y1, x2, y2);

        while (x1 <= xmax) {
                if (y1 < gridH)
                        vertical.setLine(x1, gridH, x2, y2);
                if (x1 >= gridW)
                        painter->drawLine(vertical);
                x1 += gridW;
                x2 += gridW;
                vertical.setLine(x1, y1, x2, y2);
        };

        painter->restore();
}
