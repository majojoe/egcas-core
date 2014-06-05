#ifndef WORKSHEET_H
#define WORKSHEET_H

#include <QObject>
#include <QMargins>

class Worksheet : public QObject
{
        Q_OBJECT
public:
        ///constructor of Worksheet
        explict Worksheet(QObject *parent = 0);
        virtual ~Worksheet();
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
        /**
         * @brief setDimensions set the height and width of a worksheet
         * @param dimensions the dimensions of the worksheet to be set
         */
        void setDimensions(QSizeF dimensions);
private:
        QSizeF m_grid;
        QMargins m_margins;
        QRectF m_dimensions;

};

#endif // WORKSHEET_H
