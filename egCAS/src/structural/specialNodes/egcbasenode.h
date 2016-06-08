/**Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#ifndef EGCBASENODE_H
#define EGCBASENODE_H

#include "egcunarynode.h"

/**
 * @brief The EgcBaseNode class is a class that always is the base element of an expression.
 */
class EgcBaseNode : public EgcUnaryNode
{
public:
        EgcBaseNode();
        virtual EgcNodeType getNodeType(void) const {return s_nodeType;}
protected:
        /**
         * @brief bindingPower returns the binding power of the this operation. Needs to be overridden by the user if
         * this is an operation.
         * @return -1 if the binding power is not applicable (e.g. for Number or Variable Nodes) or the binding power of
         * the operation (number >= 0).
         */
        virtual qint32 getBindingPower(void) const override;

        static const EgcNodeType s_nodeType = EgcNodeType::BaseNode;
private:
        ///copy constructor
        EgcBaseNode(const EgcBaseNode& orig) { (void) orig;}
        ///move constructor
        EgcBaseNode(EgcBaseNode&& orig) {(void) orig;}
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcBaseNode& operator=(const EgcBaseNode &rhs) {(void) rhs;}
        /**
         * @brief operator= overloads = operator since we have dynamic elements in this class (move semantic)
         * @param rhs a reference to the object to be assigned
         * @return a reference to the object the rhs is assigned to
         */
        EgcBaseNode& operator=(EgcBaseNode&& rhs) {(void) rhs;}
        ///made the copy method private to be not able to use it
        virtual EgcNode* copy(void) {return new (std::nothrow) EgcBaseNode(static_cast<const EgcBaseNode&>(*this));}
        ///made the create method private to be not able to use it
        static EgcNode* create(void) {return new (std::nothrow) EgcBaseNode();}
};

#endif // EGCBASENODE_H
