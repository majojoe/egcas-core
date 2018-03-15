/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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
#ifndef FORMULAMODIFICATOR_H
#define FORMULAMODIFICATOR_H

#include "../visitor/formulascrelement.h"
#include "../iterator/formulascriter.h"
#include <structural/actions/egcaction.h>

class EgcFormulaEntity;

/**
 * @brief The NodeIterReStructData class is intended for use during restructering a formula -> state does not change but
 * node pointers
 */
class NodeIterReStructData {
public:
        NodeIterReStructData() : m_node{nullptr} {}
        EgcNode* m_node;
        EgcNode* m_iterPosAfterUpdate;
};


class FormulaModificator
{
public:
        ///std constructor
        FormulaModificator(EgcFormulaEntity& formula);
        virtual ~FormulaModificator();
        /**
         * @brief cursorAtBegin checks if the cursor is at the beginning of a formula
         * @return true if the cursor is at the beginning of the formula, false otherwise
         */
        bool cursorAtBegin(void);
        /**
         * @brief cursorAtEnd checks if the cursor is at the end of the formula
         * @return true if the cursor is at the end of the formula, false otherwise
         */
        bool cursorAtEnd(void);
        /**
         * @brief insertCharacter insert a character at the current cursor position
         * @param character the character to insert
         */
        void insertCharacter(QChar character);
        /**
         * @brief showCurrentCursor show the cursor of the currently active formula at current position
         */
        void showCurrentCursor(void);
        /**
         * @brief viewHasChanged is to be called when view has changed to complete insert or remove operation
         */
        void viewHasChanged(void);
        /**
         * @brief insertOperation insert a operation into the tree
         * @param operation the operation action to insert
         */
        void insertOperation(EgcAction operation);
        /**
         * @brief moveCursor move cursor forward or backward
         * @param forward if true, cursor will be moved forward, backward otherwise
         */
        void moveCursor(bool forward);
        /**
         * @brief insertBinaryOperation insert a binary operation
         * @param op the binary operation to insert as ASCII string that can be interpreted by the parser
         */
        void insertBinaryOperation(QString op, QString left = QString(), QString right = QString());
        /**
         * @brief removeElement remove a character or operation at the current cursor position
         * @param previous if true the character before the current cursor position is remove, if false the character behind
         */
        void removeElement(bool previous);
        /**
         * @brief insertSegment inserts a complete segment of elements. This does not need to be done in a per elements
         * way, since the parser will split that up again correctly.
         * @param segment the segment to be insert (can consist of many elements)
         */
        void insertUnaryOperation(QString segment);
        /**
         * @brief insertElement insert an element
         * @param segment the segment to insert
         * @param left true if the element is on the left side, false otherwise
         */
        void insertUnaryElement(QString segment, bool left = true);
        /**
         * @brief insertSigns insert signs in the vector
         * @param segment the signs (text) to insert
         */
        void insertSigns(QString signs);
        /**
         * @brief insertParenthesis insert red parenthesis and change them to normal parenthesis as needed
         * @param left true if the left parenthesis is to insert, false otherwise
         */
        void insertRedParenthesis(bool left);
        /**
         * @brief createSubscript create a subscipt for a formula variable at the current cursor position
         */
        void createSubscript(void);
        /**
         * @brief toFront positions the cursor at the front of the formula
         */
        void toFront(void);
        /**
         * @brief toBack positions the cursor at the back of the formula
         */
        void toBack(void);
        /**
         * @brief updateFormula updates everything needed (view and structure) to reflect the changes
         * @return true if the change was successful, false if not
         */
        bool updateFormula(void);
        /**
         * @brief setCursorPos set the cursor position at the given node and subposition
         * @param nodeId the node where to set the cursor at
         * @param subPos the subposition where to set the subposition at
         * @param rightSide true if the cursor shall be set at the right side of the element, false if on the left side
         */
        void setCursorPos(quint32 nodeId, quint32 subPos, bool rightSide);
        /**
         * @brief markParent highlight the next parent upwards in the tree and move the cursor to that position.
         */
        void markParent(void);

private:
        /**
         * @brief resetUnderline reset the underline
         */
        void resetUnderline(void);
        /**
         * @brief isUnderlineActive checks if any underline is active
         * @return true if underline is active, false otherwise
         */
        bool isUnderlineActive(void);
        /**
         * @brief id the mathml id of the currently active node
         * @return the mathml id
         */
        quint32 id(EgcNode* node = nullptr) const;
        /**
         * @brief rightSide checks if node for representing the cursor is on the right side of the current cursor or
         * node or on the left side (Cursor is a Java Style cursor and has left and a right node).
         * @return true if on the right side, false if on the left
         */
        bool rightSide(void) const;
        /**
         * @brief nodeAtCursor returns the node at the current cursor
         * @return the node at the current cursor
         */
        EgcNode& nodeAtCursor(void) const;
        /**
         * @brief elementAtCursor returns the element at the current cursor
         * @return the formula element at current cursor position
         */
        FormulaScrElement& elementAtCursor(void) const;
        /**
         * @brief subPosition returns the subPosition of the current mathml id
         * @return the subPosition of the current mathml id
         */
        quint32 subPosition(void) const;
        /**
         * @brief reStructureTree restructure the tree e.g. after an insert or delete
         * @return true if restructing the tree was successful (insert or delete was successful)
         */
        bool reStructureTree(void);
        /**
         * @brief insertEmptyNode insert an empty node at the current cursor position
         */
        void insertEmptyNode(void);        
        /**
         * @brief insertBinEmptyNode insert an empty binary node at the current cursor position
         */
        void insertBinEmptyNode(void);
        /**
         * @brief insertEl insert an element with the given value
         * @param el the value to insert
         */
        void insertEl(QString el);
        /**
         * @brief isEmpty checks if formula is currently empty (contains just an empty element)
         * @return true if formula is empty, false if not
         */
        bool isEmpty(void) const;
        /**
         * @brief isEmptyElement checks if the previous or next element is an empty element
         * @param previous if true the previous element will be checked, if false the next element will be checked. If
         * there is no element at all, the result is false.
         * @return true if the checked element is an empty element
         */
        bool isEmptyElement(bool previous = true) const;
        /**
         * @brief isVarsubscriptSeparator checks if the previous formula element is a variable subscript separator element
         * @param previous if true (default) the previous element will be checked, if false the next element will be
         * checked. If there is no element at all, the result is false.
         * @return true if the checked element is an empty element
         */
        bool isVarsubscriptSeparator(bool previous = true) const;
        /**
         * @brief isSpecificElement checks if the previous or next element is the element given with parameter element
         * @param previous if true (default) the previous element will be checked, if false the next element will be
         * checked. If there is no element at all, the result is false.
         * @param element the element to check for
         * @return true if the checked element is an empty element
         */
        bool isSpecificElement(QString element, bool previous = true) const;
        /**
         * @brief sanitizeBinary sanitize binary operations after removing elements
         */
        void sanitizeBinary(void);
        /**
         * @brief sanitizeUnary sanitize unary operations after removing elements
         */
        void sanitizeUnary(void);
        /**
         * @brief sanitizeFlex sanitize flex operations after removing elements
         */
        void sanitizeFlex(void);
        /**
         * @brief sanitizeWithEmptyBinaryOps sanitize unary, number and variable elements after removing binary
         * operations
         */
        void sanitizeWithEmptyBinaryOps(void);
        /**
         * @brief sanitizeMisc sanitize misc operations and other stuff after especially after removing elements
         */
        void sanitizeMisc(void);
        /**
         * @brief sanitizeSpecials
         * @param el the element that has been deleted
         * @param previous true if the element has been in front of the current cursor or false if behind
         */
        void sanitizeSpecials(FormulaScrElement el, bool previous);
        /**
         * @brief rmSegmented
         * @param previous if true (default) the previous element will be removed, if false the next element will be
         * removed. If there is no segmented element at all, none will be removed.
         */
        void rmSegmented(bool previous = true);
        /**
         * @brief searchCursorPos search for a valid and suitable cursor position
         * @param iter the iterator to use to search for the cursor position
         * @param node the node to search for
         * @param subPos the subposition that shall match
         * @param rightSide true if cursor position shall be on the right side of the element
         * @param sideMatters if side of the node (left, middle, right) matters e.g. for container nodes
         * @param side if side matters the side to look for
         * @param subposRelevant if true the subposition must match
         * @return true if a valid cursor position has been found
         */
        bool searchCursorPos(FormulaScrIter& iter, EgcNode& node, quint32 subPos, bool rightSide,
                             bool sideMatters, FormulaScrElement::SideNode side, bool subposRelevant = true);
        /**
         * @brief getParent get the next visible parent of the given node
         * @param node the node to search the parent for
         * @return nullptr if there is no visible parent anymore, otherwise the visible parent of the given node
         */
        EgcNode* getParent(EgcNode& node);
        /**
         * @brief isCursorNearLeftSideParent returns true if the current cursor is near the left side of the given
         * parent
         * @return true if current cursor is near the left side of the given parent, false otherwise
         */
        bool isCursorNearLeftSideParent(EgcNode& node) const;
        /**
         * @brief moveCursorToRightVisibleBorder move cursor to the right visible border of the container node given.
         * @param node the node to where the cursor to move
         */
        void moveCursorToRightVisibleBorder(EgcNode& node);
        /**
         * @brief moveCursorToLeftVisibleBorder move cursor to the left visible border of the container node given.
         * @param node the node to where the cursor to move
         */
        void moveCursorToLeftVisibleBorder(EgcNode& node);
        /**
         * @brief getRightVisibleSide check if right side of the given node is visible
         * @param node the given node to check
         * @param iter if the return value is true, this iterator contains the position found.
         * @return true if visible, false otherwise
         */
        bool getRightVisibleSide(EgcNode& node, FormulaScrIter& iter);
        /**
         * @brief getLeftVisibleSide check if left side of the given node is visible
         * @param node the given node to check
         * @param iter if the return value is true, this iterator contains the position found.
         * @return true if visible, false otherwise
         */
        bool getLeftVisibleSide(EgcNode& node, FormulaScrIter& iter);
        /**
         * @brief split the current vector in a list with 3 vecto1rs. The original vector will not be modified!
         * @param leftIter the left (first) iterator that splits the vector in left and middle part
         * @param rightIter the right (second) iterator the splits the vector in middle and right part
         * @return a list of formula vectors
         */
        QList<FormulaScrVector> split(const FormulaScrIter& leftIter, const FormulaScrIter& rightIter) const;
        /**
         * @brief removeBinEmptyNodes removes binary emtpy nodes at the right and left side of the current cursor
         */
        void removeBinEmptyNodes(void);
        /**
         * @brief isEmptyChildNeeded4Binary checks if an empty child is needed if a binary operation shall be inserted
         * @param leftChild if true function will check for the left child of the binary operation to insert, if false
         * it will be checked for the right child
         * @return true if a empty child is needed, false if not.
         */
        bool isEmptyChildNeeded4Binary(bool leftChild);
        /**
         * @brief setMarker set a marker at the current cursor position
         */
        void setMarker(void);
        /**
         * @brief placeCursorAtMarker move the cursor to the current marker position
         */
        void placeCursorAtMarker(void);

        EgcFormulaEntity& m_formula;            ///< reference to the formula this modificator is associated with
        FormulaScrVector m_vector;              ///< vector that contains all screen data to be able to iterate over the formula
        FormulaScrIter m_iter;                  ///< the iterator associated with this modificator
        EgcNode* m_underlinedNode;              ///< the last visible parent node that was given back
        EgcNode* m_startUnderlinedNode;         ///< node where underining started
        bool m_changeAwaited;                   ///< is set if a change in the view is awaited. If this has happened we can complete the change operation
        bool m_underlineCursorLeft;             ///< if true the cursor is at left side of underline, if false on the right side
};

#endif // FORMULAMODIFICATOR_H
