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

#ifndef EGCFORMULAENTITY_H
#define EGCFORMULAENTITY_H

#include <QString>
#include <QScopedPointer>
#include "../specialNodes/egcbasenode.h"
#include "egcentity.h"
#include "egcabstractformulaentity.h"
#include "../visitor/egcmathmllookup.h"

class EgcNode;
class EgcBaseNode;
enum class EgcNodeType;
class EgcAbstractFormulaItem;
class EgcScreenPos;
class EgcScrPosIterator;
enum class EgcOperations;

/**
 * @brief The EgcNumberResultType defines different types of number results
 */
enum class EgcNumberResultType
{
        NotApplicable = 0,      ///< if this is not applicable
        StandardType,           ///< unchanged behaviour of the result
        IntegerType,            ///< result will be an integer
        EngineeringType,        ///< result will be of engineering type (exponent a multiple of 3)
        ScientificType          ///< scientific notation of the result
};

/**
 * @brief The EgcFormulaEntity class defines a wrapper for a whole equation
 */
class EgcFormulaEntity : public EgcEntity, public EgcAbstractFormulaEntity
{
        //for testing the class
        friend class EgcasTest_AdvancedTreeOps;

public:
        /**
         * @brief EgcFormulaEntity std constructor
         * @param type the type of the root element of the formula to generate
         */
        EgcFormulaEntity(EgcNodeType type);
        /**
         * @brief EgcFormulaEntity std constructor
         */
        EgcFormulaEntity(void);
        /**
         * @brief EgcFormulaEntity std constructor. Takes ownership of the rootElement.
         * @param rootElement a reference to the rootElement of the formula to create (if the root element already exists)
         */
        EgcFormulaEntity(EgcNode& rootElement);
        ///copy constructor
        EgcFormulaEntity(const EgcFormulaEntity& orig);
        ///move constructor
        EgcFormulaEntity(EgcFormulaEntity&& orig);
        ///std destructor
        virtual ~EgcFormulaEntity();
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcFormulaEntity& operator=(const EgcFormulaEntity &rhs);
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcFormulaEntity& operator=(EgcFormulaEntity&& rhs);
        /**
         * @brief operator== comparison operator overload
         * @param formula the formula to compare against
         * @return true if the trees are equal
         */
        bool operator==(const EgcFormulaEntity& formual) const;
        /**
         * @brief getBaseElement returns the base element of a formula
         * @return the root element of the formula
         */
        EgcBaseNode& getBaseElement(void) const;
        /**
         * @brief getRootElement returns the root (child of base element) element of a formula
         * @return the root element of the formula
         */
        EgcNode* getRootElement(void) const;
        /**
         * @brief setRootElement sets the root element of a formula. The formula takes ownership of the tree.
         * @param rootElement is a reference to the root Element of the formula tree to be set
         */
        void setRootElement(EgcNode *rootElement);
        /**
         * @brief getMathMlCode returns the mathMl representation for this formula
         * @return the mathMl representation of this formula as a string
         */
        QString getMathMlCode(void) override;
        /**
         * @brief getCASKernelCommand returns the command to execute the calculation(s) needed for this formula in the
         * used CAS kernel
         * @return the command to be executed in the CAS kernel as a string
         */
        QString getCASKernelCommand(void);
        /**
         * @brief Is the formula a result of a calculation? (Has to be set by the formula(visitor) - when the formula
         * changes)
         * @return true if the formula is a result of a calculation
         */
        bool isResult(void);
        /**
         * @brief Is this a result with a number as result. (Has to set/triggered by the kernel parser)
         * @return true if this is a number result
         */
        bool isNumberResult(void);
        /**
         * @brief sets the number of significant digits that is shown in the result of this formula
         * @param digits the number of digits to show in the result of this formula
         */
        void setNumberOfSignificantDigits(quint8 digits);
        /**
         * @brief returns the number of significant digits set by the user
         * @return the number of significant digits (if set and applicable) or 0 otherwise
         */
        quint8 getNumberOfSignificantDigits(void);
        /**
         * @brief sets the result type for this formula if the result is a number result
         * @param resultType the result type (engineering, scientific, integer, standard)
         */
        void setNumberResultType(EgcNumberResultType resultType);
        /**
         * @brief returns the result type of the result (if one)
         * @return returns EgcNumberResultType::NotApplicable if not a number result type and the number result type
         * set by the user otherwise
         */
        EgcNumberResultType getNumberResultType();
        /**
         * @brief returns the global number of significant digits
         * @return the global significant digits
         */
        static quint8 getStdNrSignificantDigis(void);
        /**
         * @brief set the number of global significant digits (valid in the whole document)
         * @param digits the number of global significant digits
         */
        static void setStdNrSignificantDigis(quint8 digits);
        /**
         * @brief setResult sets the tree as result of the formula. The formula takes ownership of the result, even if
         * it's not possible to set the result as result of the formula (the result given will be deleted in this case).
         * @param result the EgcNode tree to to set as result of the formula. Of course the result
         * @return true if formula needs a repaint.
         */
        bool setResult(EgcNode* result);
        /**
         * @brief resetResult if the content of the formula is a result, delete the result and set it empty
         */
        void resetResult(void);
        /**
         * @brief getEntityType returns the entity type of the current class
         * @return the entity type
         */
        virtual enum EgcEntityType getEntityType(void) const override;
        /**
         * @brief getPositon returns the position of the current entity
         * @return the position of the entity in the current worksheet
         */
        virtual QPointF getPosition(void) const override;
        /**
         * @brief setPosition set the position of a entity
         * @param pos the position where the entity should be
         */
        virtual void setPosition(QPointF pos) override;
        /**
         * @brief set the generic font size for all formulas (changes the overall font size of all formulas in a document).
         * If the font size of a specific formula should be changed, use the function setFontSize.
         * @param size the font size in points
         */
        static void setGenericFontSize(int size);
        /**
         * @brief getBaseFontSize returns the base font size of all formulas in a document
         * @return the base font size of all formulas
         */
        static int getGenericFontSize(void);
        /**
         * @brief set the font size for a formula (changes only the font size of this formula).
         * If the overall font size of all formulas should be changed, use the function setBaseFontSize.
         * @param size the font size in points
         */
        void setFontSize(int size);
        /**
         * @brief getFontSize returns the font size of the current formula
         * @return the font size of the current formula
         */
        int getFontSize(void);
        /**
         * @brief setItem set the formula item that is associated with this entity
         * @param item the item to set (can also be a nullptr)
         */
        void setItem(EgcAbstractFormulaItem* item) override;
        /**
         * @brief getItem returns a pointer to the item in the scene
         * @return pointer to item in scene
         */
        EgcAbstractFormulaItem* getItem(void) const;
        /**
         * @brief updateView update the view with the new mathml representation if anything changes
         */
        void updateView(void);
        /**
         * @brief itemChanged is called when the item that is associated with the enity has changed
         */
        virtual void itemChanged(EgcItemChangeType changeType) override;
        /**
         * @brief setErrorMessage set the error message of the formula, if one
         * @param msg the message to set
         */
        void setErrorMessage(QString msg);
        /**
         * @brief getErrorMessage returns the error message of the formula, if one
         * @return the error message
         */
        QString getErrorMessage(void);
        /**
         * @brief handleAction handles the given action (e.g. insert a char at the given position into the formula tree)
         * @param EgcAction the action given
         */
        virtual void handleAction(const EgcAction& action) override;
        /**
         * @brief getMathmlMapping returns a reference to the internal mathml id lookup table
         * @return the lookup table
         */
        EgcMathmlLookup& getMathmlMappingRef(void);
        /**
         * @brief getMathmlMapping returns a const reference to the internal mathml id lookup table
         * @return the lookup table
         */
        const EgcMathmlLookup& getMathmlMappingCRef(void) const;
        /**
         * @brief copy copies the tree downwards from the given node position and returns a pointer to the copied
         * tree. Caller needs to handle ownership of the returned tree.
         * @return the copied tree
         */
        EgcNode* copy(EgcNode& node);
        /**
         * @brief cut the tree downwards from the given node position and inserts an empty node at the place of
         * the cut out node. Caller needs to handle ownership of the returned tree.
         * @return the tree that was cut off.
         */
        EgcNode* cut(EgcNode& node);
        /**
         * @brief paste the given tree (given with the root node) at the position of the given second node.
         * @param treeToPaste the tree that will be pasted at the current iterator position.
         * @param whereToPaste the tree treeToPaste will be inserted here. The node (maybe) including the tree downwards
         * will be deleted.
         * @return true if the paste was successful, false otherwise.
         */
        bool paste(EgcNode& treeToPaste, EgcNode& whereToPaste);
        /**
         * @brief replaceEmptyNodeWith replace the empty node where currently the cursor is, with a node of the given
         * type.
         * @param type the type to use for the node as replacement for the empty node
         * @return nullptr if operation was not successful, or node inserted at cursor position where empty node was.
         */
        EgcNode* replaceEmptyNodeWith(EgcNodeType type);
        /**
         * @brief isResultNode check if the given node is part of a result from the calculation kernel
         * @param node the node to check for if it is a part of a result
         * @return true if it is a result node, false otherwise
         */
        static bool isResultNode(const EgcNode& node);
        /**
         * @brief rearrangePrecedence when a formula is modified (modify, insert or remove operators), the formula tree
         * needs to be rearranged in a sense that the operator precedence of the mathematical operation is valid again.
         * E.g. when changing a plus operator to a multiplication operator, the multiplication has a higher precedence.
         * Therefore the formula structure has to be rearranged to reflect the changed operation.
         */
        void rearrangePrecedence(void);
        /**
         * @brief isNodeInFormula checks if the given node is a node of this formula
         * @param node the node to check
         * @return true if the given node is a node of this formula, false otherwise
         */
        bool isNodeInFormula(EgcNode& node);
        /**
         * @brief cursorAtBegin checks if cursor is at beginning of formula
         * @return true if cursor is at beginning
         */
        virtual bool cursorAtBegin(void) override;
        /**
         * @brief cursorAtEnd checks if cursor is at end of formula
         * @return true if cursor is at end
         */
        virtual bool cursorAtEnd(void) override;
        /**
         * @brief setSelected sets the formula selected
         */
        void setSelected(void);
        /**
         * @brief isEmpty check if the formula is an empty formula
         * @return true if empty, false otherwise
         */
        virtual bool isEmpty(void) const;
        /**
         * @brief setCursorPos set the cursor position at the given nodeId and sub position location
         * @param nodeId the node Id where to set the cursor
         * @param subPos the sub position where to set the cursor
         * @param rightSide set it at the right side or at the left of the given instance
         */
        virtual void setCursorPos(quint32 nodeId, quint32 subPos, bool rightSide) override;

private:
        /**
         * @brief moveCursor move the screen formula cursor back- or forward
         * @param forward true if the user wants to move the cursor forward, false if he wants to move it backward
         */
        void moveCursor(bool forward);
        /**
         * @brief showCurrentCursor shows the current cursor the iterator points to
         */
        void showCurrentCursor(void);
        /**
         * @brief markParent highlight the next parent upwards in the tree and move the cursor to that position.
         */
        void markParent(void);
        /**
         * @brief insertCharacter insert a character at the current cursor position
         * @param character the character to insert
         */
        void insertCharacter(QChar character);
        /**
         * @brief insertOperation insert a operation into the tree
         * @param operation the operation action to insert
         */
        void insertOperation(EgcAction operation);
        /**
         * @brief removeCharacter remove a character at the current cursor position
         * @param before if true the character before the current cursor position is remove, if false the character behind
         */
        void removeCharacter(bool before);
        /**
         * @brief isScreenIterInSubtree check if screen iterator is in the given subtree.
         * @param tree subtree to check if screen iterator is anywhere below.
         * @param rightSide if screen iterator is in given subtree, return the side that is nearer. True if right is 
         * nearer, false if left side is nearer
         * @return true if screen iterator is in subtree, false otherwise
         */
        bool isScreenIterInSubtree(EgcNode& tree, bool &rightSide) const;
        /**
         * @brief rearrangeOnePrecedence rearanging the precedence of a formula tree needs to be done in an iterative
         * way. This rearanges just one operation in the tree if there is s.th. to rearange.
         * @return true if the end of the tree has been reached without rearrainging a node, false otherwise
         */
        bool rearrangeOnePrecedence(void);
        /**
         * @brief rotateTreeRight rotates the subtree of this node right (see tree threory -> discrete mathematics)
         * @return true if right rotation was possible and done, false otherwise.
         */
        virtual bool rotateTreeRight(EgcNode& treeNodeToRotate);
        /**
         * @brief rotateTreeLeft rotates the subtree of this node left (see tree threory -> discrete mathematics)
         * @return true if left rotation was possible and done, false otherwise.
         */
        virtual bool rotateTreeLeft(EgcNode& treeNodeToRotate);
        /**
         * @brief isEmptyNode check if current node where cursor is, is an empty node or not
         * @return true if it is an empty node, false otherwise
         */
        bool isEmptyNode(void);
        /**
         * @brief insertOp insert any operation at current position of the iterator
         * @param operations the operation action to insert
         * @return true if the insert operation was successful, false otherwise
         */
        bool insertOp(EgcAction operations);
        /**
         * @brief createAndInsertOp insert a unary node in the tree above the given node (position)
         * @param type the node type to insert
         * @return true if everything worked well, false otherwise.
         */
        bool createAndInsertOp(EgcNodeType type);
        /**
         * @brief createAndInsertOperation insert a unary node in the tree above the given node (position)
         * @param type the node type to insert
         * @return pointer to node inserted if everything worked well, nullptr otherwise.
         */
        EgcNode* createAndInsertOperation(EgcNodeType type);

        quint8 m_numberSignificantDigits;       ///< number of significant digits of a number result
        EgcNumberResultType m_numberResultType; ///< the style how the number result shall be presented to the user
        EgcBaseNode m_data;                     ///< holds a pointer to the root element of the formula tree
        static quint8 s_stdNrSignificantDigits; ///< the number of significant digits (in a global mannner (std))
        EgcAbstractFormulaItem* m_item;         ///< pointer to the formula item interface on the scene
        QString m_errorMsg;                     ///< the error message for this formula, if one
        EgcMathmlLookup m_mathmlLookup;         ///< mathml id lookup table
        QScopedPointer<EgcScrPosIterator> m_scrIter; ///< iterator for screen positions
};

#endif // EGCFORMULAENTITY_H
