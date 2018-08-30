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

#ifndef FORMULAITEM_H
#define FORMULAITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include "egcasiteminterface.h"
#include "egcabstractformulaitem.h"
#include "egcabstractitem.h"
#include "actions/egcaction.h"

class EgMathMLDocument;
class EgcAbstractFormulaEntity;
class EgcAbstractFormulaItem;
class EgcScreenPos;

/**
 * @brief The FormulaItem class implements a QGraphicsItem to be able to use a formula in a QGraphicsView
 */
class EgcFormulaItem : public QGraphicsItem, public EgcAbstractFormulaItem, public EgcAbstractItem
{
public:        
        ///standard constructor
        explicit EgcFormulaItem(QGraphicsItem *parent = 0);
        virtual ~EgcFormulaItem();
        /**
         * @brief constructor for instantiating a formula with xml representation already available
         * @param point the left bottom start point for rendering the formula
         * @param parent pointer to parent widget
         */
        explicit EgcFormulaItem(const QPointF point, QGraphicsItem *parent = 0);
        /**
         * @brief constructor for instantiating a formula with xml representation already available
         * @param formula the mathml representation of the formula to render
         * @param point the left bottom start point for rendering the formula
         * @param parent pointer to parent widget
         */
        explicit EgcFormulaItem(const QString &formula, QPointF point, QGraphicsItem *parent = 0);
        /**
         * @brief override paint from QGraphicsItem
         * @param painter pointer to painter
         * @param option pointer to style options
         * @param widget pointer to parent widget
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
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
         * @brief setEntity set a pointer to the entity that contains the logical structure / frontend for the view
         * @param entity a pointer to the entity that is associated with this object
         */
        void setEntity(EgcAbstractFormulaEntity* entity);
        /**
         * @brief getEnity returns a pointer to the entity that contains the logical structure for the formula
         * @return the entity that is associated with the formula item
         */
        EgcAbstractFormulaEntity* getEnity(void) const;
        /**
         * @brief getPosItemIface needs to be overwritten by subclasses to get the position of the item
         * @return the Position of the item
         */
        virtual QPointF getPosition( void ) const override;
        /**
         * @brief setPosItemIface needs to be overwritten by subclasses to set the position of the item
         * @param point the position to set.
         */
        virtual void setPos(const QPointF &pos) override;
        /**
         * @brief updateView update the view with the new mathml representation if anything changes
         */
        virtual void updateView(void) override;
        /**
         * @brief getScreenPos returns a reference to the object that manages the screen positions of the formula
         * characters
         * @return a reference to the screen positions
         */
        virtual const EgcScreenPos& getScreenPos(void) const;
        /**
         * @brief paintUnderline paint the underline that marks any mathml node adressed by the mathml id, to be able
         * to show the user the context of his operation (e.g. keystroke).
         * @param mathmlId the mathml id for which to paint a underline
         */
        virtual void showUnderline(quint32 mathmlId) override;
        /**
         * @brief showLeftCursor shows the cursor on the left side of a mathml element
         * @param mathmlId the mathml id to use for identifying the mathml element
         * @param subindex the subindex to use for identifying the mathml element (e.g. one character of a text)
         */
        virtual void showLeftCursor(quint32 mathmlId, quint32 subindex) override;
        /**
        * @brief showRightCursor shows the cursor on the right side of a mathml element
        * @param mathmlId the mathml id to use for identifying the mathml element
        * @param subindex the subindex to use for identifying the mathml element (e.g. one character of a text)
        */
        virtual void showRightCursor(quint32 mathmlId, quint32 subindex) override;
        /**
         * @brief getElementRect returns the bounding rect of the requested element. If no element with the given
         * parameters is availiable an empty rect will be returned
         * @param mathmlId the mathml id for which the bounding rect is requested
         * @param subindex the subindex for which the bounding rect is requested
         * @return the bounding rect for given mathml id and subindex (in formula coordinates)
         */
        virtual QRectF getElementRect(quint32 mathmlId, quint32 subindex) override;
        /**
         * @brief showCursor shows the cursor given in the context of the current scene
         * @param cursor the cursor to show up
         */
        virtual void showCursor(QLineF cursor) override;
        /**
         * @brief hideCursors hide the cursors e.g. in case the element is moved
         */
        virtual void hideCursors(void) override;

        /**
         * @brief selectFormula select the formula and grab focus
         * @param selected if true formula gets selected, if false deselected
         */
        virtual void selectFormula(bool selected) override;
        /**
         * @brief type returns the type of the item
         * @return item type
         */
        virtual int type() const override;
        /**
         * @brief setCursorAt set the cursor position near the given position
         * @param pos the position where to set the cursor
         */
        void setCursorAt(QPointF pos);

protected:
        /**
         * @brief getEgcScene needs to be implemented by the subclasses since we cannot inherit from QGraphicsitem (the
         * subclasses already inherit from it - and we don't want to make it complicated)
         * @return pointer to EgCasScene
         */
        virtual EgCasScene* getEgcScene(void) override;
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
         * @brief mouseDoubleClickEvent overrides the double click event form QGraphicsItem
         * @param event pointer to QGraphicsSceneMouseEvent
         */
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
        /**
         * @brief itemChange reimplements change function of QGraphicsItem to be able to realize a grid
         * @param change enum that describes state changes that are notified
         * @param value the value that has changed
         * @return the value that has been adjusted
         */
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        /**
         * @brief focusInEvent overwrite focus in event to be able to work with the position iterator
         * @param event focus event
         */
        virtual void focusInEvent(QFocusEvent * event) override;
        /**
         * @brief focusOutEvent overwrite focus in event to be able to work with the position iterator
         * @param event focus event
         */
        virtual void focusOutEvent(QFocusEvent * event) override;
        /**
         * @brief keyPressEvent overrides key press event from QGraphicsItem
         * @param event contains the key pressed
         */
        void keyPressEvent(QKeyEvent * event) override;
        /**
         * @brief keyReleaseEvent overrides key release event from QGraphicsItem
         * @param event contains the key released
         */
        void keyReleaseEvent(QKeyEvent * event) override;
        /**
         * @brief keyCursorKeyHandler overwrites key events
         * @param keyEvent the key event to react on
         */
        void keyCursorKeyHandler(QKeyEvent *keyEvent);
        /**
         * @brief isAtLeftEnd checks if cursor is at the left end of the text field
         * @return true if cursor is at the left end, false otherwise
         */
        bool isAtLeftEnd(void);
        /**
         * @brief isAtRightEnd checks if cursor is at the right end of the text field
         * @return true if cursor is at the right end, false otherwise
         */
        bool isAtRightEnd(void);
        /**
         * @brief bRect returns the bounding rect of the abstract item (interface to concrete item)
         * @return bounding rect rectangle
         */
        virtual QRectF bRect(void) const override;
        /**
         * @brief getPos return the current position of the item
         * @return the current position of the item
         */
        virtual QPointF getPos(void) override;
        /**
         * @brief setEditMode set the formula in edit mode or leave edit mode
         * @param edit if true formula is set to edit mode, if false formula leaves edit mode
         */
        void setEditMode(bool edit);
signals:

public slots:
private:                
        QScopedPointer<EgMathMLDocument> m_mathMlDoc;
        static quint8 s_baseFontSize;
        EgcAbstractFormulaEntity* m_entity;             ///< pointer to formula entity
        bool m_posChanged;                              ///< helper variable indicating that the position has changed
        bool m_contentChanged;                          ///< helper variable indicating that the content of the formula has changed
        QScopedPointer<EgcScreenPos> m_screenPos;       ///< screen positions of the rendered formula characters
        static QRegularExpression s_alnumKeyFilter;     ///< regex for checking if a key is an alnum key
        static bool s_regexInitialized;                 ///< check if regex is already initialized
        QPointF m_startPoint;                           ///< point where move operation started
        bool m_movePossible;                            ///< move is possible at the moment
        bool m_editingActivated;                        ///< true if editing is activated

        Q_DISABLE_COPY(EgcFormulaItem)
};

#endif // FORMULAITEM_H
