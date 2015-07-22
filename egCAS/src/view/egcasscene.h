/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
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

#ifndef EGCASSCENE_H
#define EGCASSCENE_H

#include <QGraphicsScene>

class EgcFormulaItem;

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
        EgcFormulaItem * addFormula(const QString & formula, int size);
protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect);
private:
        QSizeF m_grid;
};

#endif // EGCASSCENE_H
