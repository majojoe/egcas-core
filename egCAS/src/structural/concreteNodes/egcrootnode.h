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

#ifndef EGCROOTNODE_H
#define EGCROOTNODE_H

#include "../specialNodes/egcbinaryoperator.h"

/**
 * @brief The EgcRootNode class is a class for root operations. This manages e.g. something like sqrt(10).
 */
class EgcRootNode : public EgcBinaryOperator
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcRootNode, EgcNodeType::RootNode);
public:
        EgcRootNode();
        /**
         * @brief cursorSnaps find out where a cursor will snap in (e.g. a division node will snap at right and at the
         * left side of the container)
         * @param side the side to test for cursor snap.
         * @return true if the cursor will snap in at the given side, false otherwise
         */
        virtual bool cursorSnaps(EgcNodeSide side) const override;
protected:
        /**
         * @brief bindingPower returns the binding power of the this operation. Needs to be overridden by the user if
         * this is an operation.
         * @return -1 if the binding power is not applicable (e.g. for Number or Variable Nodes) or the binding power of
         * the operation (number >= 0).
         */
        virtual qint32 getBindingPower(void) const override;
        /**
         * @brief getReordProtectSide returns the sides where reordering protectors of a special node are
         * @return the sides where reordering protectors are
         */
        virtual ReordProtectSide getReordProtectSide(void) const override;
};

#endif // EGCROOTNODE_H
