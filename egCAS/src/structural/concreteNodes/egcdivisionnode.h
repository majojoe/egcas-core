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

#ifndef EGCDIVISIONNODE_H
#define EGCDIVISIONNODE_H

#include "../specialNodes/egcbinarynode.h"

/**
 * @brief The EgcDivisionNode class is a class for division operations. 
 */
class EgcDivisionNode : public EgcBinaryNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcDivisionNode, EgcNodeType::DivisionNode);
public:
        EgcDivisionNode();
        /**
         * @brief cursorSnaps find out where a cursor will snap in (e.g. a division node will snap at right and at the
         * left side of the container)
         * @param side the side to test for cursor snap.
         * @return true if the cursor will snap in at the given side, false otherwise
         */
        virtual bool cursorSnaps(EgcNodeSide side) const override;
        /**
         * @brief setChild set the given expression as a child at position index. Takes ownership of the node given,
         * even if setting the child failed (the given node will be deleted in this case).
         * @param index the position at which the child should be inserted. E.g. 0 will set the left child of a binary
         * expression.
         * @param expression the expression to set as child.
         * @return true if everything went well, false if index is > getNumberChildNodes() - 1
         */
        virtual bool setChild(quint32 index, const EgcNode& expression) override;
};

#endif // EGCDIVISIONNODE_H
