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

#ifndef EGCBINARYOPERATOR_H
#define EGCBINARYOPERATOR_H

#include "egcbinarynode.h"


/**
 * @brief The EgcBinaryOperator class is a base class for operations that takes two arguments (binary) as subexpressions.
 * This can be e.g. a multiplication or substraction.
 */
class EgcBinaryOperator : public EgcBinaryNode
{
public:
        ///std contructor
        EgcBinaryOperator();
        /**
         * @brief setChild set the given expression as a child at position index. Takes ownership of the node given,
         * even if setting the child failed (the given node will be deleted in this case).
         * @param index the position at which the child should be inserted. E.g. 0 will set the left child of a binary
         * expression.
         * @param expression the expression to set as child.
         * @return true if everything went well, false if index is > getNumberChildNodes() - 1
         */
        virtual bool setChild(quint32 index, EgcNode& expression) override;
        /**
         * @brief hasReorderingProtector checks if this binary operator has a reordering protector (invisible
         * parenthesis) to protect against operator reordering e.g. for Divisor of a division operation
         * @param index the child index to test left = 0, right = 1
         * @return true if reordering protector is there, false otherwise
         */
        virtual bool hasReorderingProtector(quint32 index) const override;
        /**
         * @brief isLeftAssociative returns wether the operation is left or right associative. This is only applicable
         * if it is an operation.
         * @return returns true if the operation is left associative, false if it is right associative.
         */
        virtual bool isLeftAssociative(void) const;
        /**
         * @brief isReorderingProtector test if the given node is a reordering protector. A user will likely never need
         * this function
         * @param node the node to test for reordering protection functionality
         * @return true if the given node is a reordering protection node, false otherwise
         */
        static bool isReorderingProtector(EgcNode& node);

protected:
        /**
         * @brief The ReordProtectSide enum defines the sides on which reordering protectors can be
         */
        enum class ReordProtectSide {none = 0, leftProtector = 0x1, rightProtector = 0x2, rightAndLeftProtector = 0x3};

        /**
         * @brief allocReorderingProtector allocates invisible parenthesis for left and/or right childs. This is e.g.
         * the case with Division Nodes. When reordering operations The childs of a division may contain suboperations
         * such as Additions. These may not be put outside of the division operation (lower binding power) during
         * reordering when the user typed them e.g. on the fraction sign. If reordering would be allowed for them, they
         * would appear beside the fraction sign after reordering.
         * @return true if everything went well, false otherwise
         */
        bool allocReorderingProtector(void);
        /**
         * @brief getReordProtectSide returns the sides where reordering protectors of a special node are
         * @return the sides where reordering protectors are
         */
        virtual ReordProtectSide getReordProtectSide(void) const;
};

#endif // #ifndef EGCBINARYOPERATOR_H
