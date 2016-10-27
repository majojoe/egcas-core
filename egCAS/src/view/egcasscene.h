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
#include <QGraphicsLineItem>
#include <QScopedPointer>
#include <QLine>
#include "entities/egcabstractformulaentity.h"
#include "entities/egcabstractpixmapentity.h"
#include "entities/egcabstracttextentity.h"

class EgcFormulaItem;
class EgcPixmapItem;
class EgcTextItem;
class EgcCrossItem;

class EgCasScene : public QGraphicsScene
{
        Q_OBJECT
public:
        ///constructor of EgCasScene
        explicit EgCasScene(QObject *parent = 0);
        virtual ~EgCasScene();
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
         * @brief addText add a text to the graphicsscene
         * @param text the text entity to be rendered
         * @param point  the point where (position) to add the text on the scene
         * @return a pointer to the text added
         */
        EgcTextItem* addText(EgcAbstractTextEntity& text, QPointF point = QPointF(0.0,0.0));
        /**
         * @brief addPixmap add a pixmap to the scene (overrides the standard function and adds a egcaspixmapitem)
         * @param pixmap the pixmap to add to the scene
         * @return a pointer to the pixmap item
         */
        QGraphicsPixmapItem * addPixmap(const QPixmap & pixmap);
        /**
         * @brief addPixmap  add a pixmap to the graphicsscene
         * @param pixmap the pixmap entity to be rendered
         * @param point  the point where (position) to add the pixmap on the scene
         * @return a pointer to the pixmap added
         */
        EgcPixmapItem* addPixmap(EgcAbstractPixmapEntity& pixmap, QPointF point = QPointF(0.0,0.0));
        /**
         * @brief addFormula add a formula to the graphicsscene
         * @param formula the formula to be rendered
         * @param size the font size in pixel of the forumla
         * @param point the point where (position) to add the formula on the scene
         * @return a pointer to the formula added
         */        
        EgcFormulaItem * addFormula(const QString & formula, int size, QPointF point);
        /**
         * @brief addFormula  add a formula to the graphicsscene
         * @param formula the formula entity to be rendered
         * @param point  the point where (position) to add the formula on the scene
         * @return a pointer to the formula added
         */
        EgcFormulaItem* addFormula(EgcAbstractFormulaEntity& formula, QPointF point = QPointF(0.0,0.0));
        /**
         * @brief setFormulaCursor shows the cursor for modifying a formula at the given position
         * @param line the position where to paint the cursor
         */
        void setFormulaCursor(const QLineF& line);
        /**
         * @brief setUnderlineCursor shows the formula underline cursor at the given position
         * @param line the position where to paint the underline cursor
         */
        void setUnderlineCursor(const QLineF& line);
        /**
         * @brief hideFormulaCursors hides all formula cursors
         */
        void hideFormulaCursors( void );
protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect);
        /**
         * @brief mouseReleaseEvent reimplements mouse release event to show cursor
         * @param event mouse event
         */
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
        /**
         * @brief keyReleaseEvent overwrites key events
         * @param keyEvent the key event to react on
         */
        virtual void keyPressEvent(QKeyEvent *keyEvent) override;
        /**
         * @brief focusOutEvent overwrite focus in event to be able to work with the position iterator
         * @param event focus event
         */
        virtual void focusOutEvent(QFocusEvent * event) override;

private:
        /**
         * @brief handleCursorEvent handles cross cursor events
         * @param keyEvent the key event that occurred
         */
        void handleCursorEvent(QKeyEvent *keyEvent);

        QSizeF m_grid;
        QGraphicsLineItem* m_cursor;             ///< formula cursor for modifying formula
        QGraphicsLineItem* m_nodeUnderline;      ///< node cursor to show user the context of changes in a formula
        EgcCrossItem* m_cross;                   ///< crosshair (cursor) to be able to see enter position
};

#endif // EGCASSCENE_H
