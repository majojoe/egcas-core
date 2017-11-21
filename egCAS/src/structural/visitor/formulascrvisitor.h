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
         * @param node the node that is associated with this element
         * @param cursorAdhesion can be given to change the standard cursor Adhesion which is normal
         */
        void append(QString str, EgcNode* node, CursorAdhesion cursorAdhesion = CursorAdhesion::low);
        /**
         * @brief appendRaw append a formula element to the internal vector without incrementing the id
         * @param str the formula element to add
         * @param node the node that is associated with this element
         * @param id the id to use for the given node
         * @param cursorAdhesion can be given to change the standard cursor Adhesion which is normal
         */
        void appendRaw(QString str, EgcNode* node, quint32 id, CursorAdhesion cursorAdhesion = CursorAdhesion::low);
        /**
         * @brief append append a number of formula signs to the internal vector (e.g. a variable name)
         * @param str the formula element to add
         * @param node the node that is associated with this the signs
         * @param cursorAdhesion can be given to change the standard cursor Adhesion which is normal
         */
        void appendSigns(QString str, EgcNode* node, CursorAdhesion cursorAdhesion = CursorAdhesion::low);
        /**
         * @brief append append a formula element that is segmented in several parts to the internal vector. When
         * deleting one of the parts later, appended here, the all parts of the node will be deleted
         * @param str the formula element to add
         * @param node the node that is associated with this element
         * @param cursorAdhesion can be given to change the standard cursor Adhesion which is normal
         */
        void appendSegmented(QString str, EgcNode* node, CursorAdhesion cursorAdhesion = CursorAdhesion::low);
        /**
         * @brief suppressNode suppress the given node (cursor will not react on it)
         * @param node the node to supress
         */
        void suppressNode(const EgcNode* node);

        FormulaScrIter& m_iter;                 ///< iterator for iterating and modifying all FormulaScrElement elements
        quint32 m_id;                           ///< id counter during visitor run
        QHash<EgcNode*, quint32> m_hash;        ///< hash for lookup of id's of parts of a node
        QSet<const EgcNode*> m_suppressList;          ///< a list with pointers EgcNode elements that shall not be rendered

};



#endif // FORMULASCRVISITOR_H
