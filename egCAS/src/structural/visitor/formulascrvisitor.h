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
#ifndef FORMULASCRVISITOR_H
#define FORMULASCRVISITOR_H

#include "../iterator/egcnodeiterator.h"
#include "egcnodevisitor.h"
#include "formulascrelement.h"
#include <QtGlobal>
#include <QHash>

class EgcFormulaEntity;
class FormulaScrElement;
class EgcNode;
class FormulaScrIter;

class FormulaScrVisitor : public EgcNodeVisitor
{
public:
        /**
         * @brief EgcNodeVisitor std constructor for the visitor
         * @param formula the formula to be parsed
         * @param vector the vector where the result shall be saved
         */
        FormulaScrVisitor(EgcFormulaEntity& formula, FormulaScrIter& iter);
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcBinaryNode* binary) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcUnaryNode* unary) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param flex the node with the information to be extracted.
         */
        virtual void visit(EgcFlexNode* flex) override;
        /**
         * @brief visit this method is called from the current node and implements the code that extracts the
         * necessary information from the node given.
         * @param binary the node with the information to be extracted.
         */
        virtual void visit(EgcNode* node) override;
        /**
         * @brief getResult returns the result of the traversion of the tree
         * @return the result of the traversion as string
         */
        virtual QString getResult(void) override;
        /**
         * @brief updateVector update the vector the visitor is referencing. This must be done after the AST of the
         * formula has been updated.
         */
        void updateVector(void);
        /**
         * @brief setElementSubposition set the subpostion of the current element. This is neccessary to be able to find
         * the cursor positions by clicking at an element. Warning: This function must be called right after the element
         * has been added e.g. by calling append.
         * @param subposition the (minimum) subposition of a sign that can be mapped to the current element.
         * @param maxSubposition the maximum subposition of a sign that can be mapped to the current element.
         */
        void setElementSubposition(quint32 subposition, quint32 maxSubposition = 0);
private:
#warning let the absolute neccessary functions in the EgcNodeVisitor class and put the implementation specific functions in a implementation class later (EgcNodeVisitor->accept is needed in this class, so we have to inherit from EgcNodeVisitor)
        /** hide all of the following functions* /
        virtual void assembleResult(QString formatString, EgcNode* node);
        virtual void assembleResult(QString startString, QString seperationString, QString endString, EgcNode* node);
        virtual void assembleResult(QString lStartString, QString rStartString, QString seperationString,
                                            QString endString, EgcNode* node);
        virtual void pushToStack(QString str, EgcNode* node);
        virtual void suppressChild(const EgcNode* node, quint32 index);
        virtual void suppressThis(const EgcNode* node);
        virtual EgcNode* getChildToSuppress(const EgcNode* node, quint32 index);
        virtual void deleteFromStack(int nrStackObjects);
        virtual QVector<QString> getAssembleArguments(EgcNode* node);
        virtual QString& modifyNodeString(QString &nodeString, EgcNode* node);

        /**
         * @brief append append a formula element to the internal vector
         * @param str the formula element to add
         * @param node whose bounding rect describes in combination with the subpos parameter the left cursor of the
         * element. If the rNode parameter is not given it also describes the right cursor.
         * @param cursorAdhesion can be given to change the standard cursor Adhesion which is normal
         * @param subpos subposition that can be set to reflect the correct position rectangle. Some nodes have multiple
         * bounding rects to describe an element.
         * @param leftSide true if left edge of the given bounding rect (associated with node) shall be used as left
         * cursor of the element, false otherwise
         * @param rNode if this parameter is given the right cursor is determined by the bounding rect of the given node.
         * @param rSubpos subposition that can be set to reflect the correct position rectangle. Some nodes have multiple
         * bounding rects to describe an element.
         * @param rLeftSide true if the left side of the given bounding rect (associated with rNode) shall be used as
         * right cursor of the element, false otherwise
         */
        void append(QString str, EgcNode* node, CursorAdhesion cursorAdhesion = CursorAdhesion::low, quint32 subpos = 0,
                    bool leftSide = true, EgcNode* rNode = nullptr, quint32 rSubpos = 0, bool rLeftSide = false);
        /**
         * @brief append append a formula element to the internal vector. Same as above, but this element is segmented
         * in multiple ones. So if deleting such an element later, all of the others will also be deleted.
         * @param str the formula element to add
         * @param node whose bounding rect describes in combination with the subpos parameter the left cursor of the
         * element. If the rNode parameter is not given it also describes the right cursor.
         * @param cursorAdhesion can be given to change the standard cursor Adhesion which is normal
         * @param subpos subposition that can be set to reflect the correct position rectangle. Some nodes have multiple
         * bounding rects to describe an element.
         * @param leftSide true if left edge of the given bounding rect (associated with node) shall be used as left
         * cursor of the element, false otherwise
         * @param rNode if this parameter is given the right cursor is determined by the bounding rect of the given node.
         * @param rSubpos subposition that can be set to reflect the correct position rectangle. Some nodes have multiple
         * bounding rects to describe an element.
         * @param rLeftSide true if the left side of the given bounding rect (associated with rNode) shall be used as
         * right cursor of the element, false otherwise
         */
        void appendSegmented(QString str, EgcNode* node, CursorAdhesion cursorAdhesion = CursorAdhesion::low, quint32 subpos = 0,
                    bool leftSide = true, EgcNode* rNode = nullptr, quint32 rSubpos = 0, bool rLeftSide = false);
        /**
         * @brief append append a number of formula signs to the internal vector (e.g. a variable name)
         * @param str the formula element to add
         * @param node the node that is associated with this the signs
         * @param cursorAdhesion can be given to change the standard cursor Adhesion which is normal
         */
        void appendSigns(QString str, EgcNode* node, CursorAdhesion cursorAdhesion = CursorAdhesion::low);
        /**
         * @brief suppressNode suppress the given node (cursor will not react on it)
         * @param node the node to supress
         */
        void suppressNode(const EgcNode* node);
        /**
         * @brief doPostprocessing does some postprocessing on the result vector
         */
        void doPostprocessing(void);

        FormulaScrIter& m_iter;                 ///< iterator for iterating and modifying all FormulaScrElement elements
        QSet<const EgcNode*> m_suppressList;    ///< a list with pointers EgcNode elements that shall not be rendered
        EgcNode* m_currNode;                    ///< node that currently is in processing

};



#endif // FORMULASCRVISITOR_H
