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

#ifndef EGCFUNCTIONNODE_H
#define EGCFUNCTIONNODE_H

#include <QString>
#include "../specialNodes/egcflexnode.h"

/**
 * @brief The EgcFuntionNode class is a class to model function calls. This manages e.g. something like calculation1(10, x + 3, 9.8234).
 */
class EgcFunctionNode : public EgcFlexNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcFunctionNode, EgcNodeType::FunctionNode);
public:
        EgcFunctionNode();
        /**
         * @brief setName set the function name
         * @param fncName the variable name as a string
         */
        virtual void setName(const QString& fncName);
        /**
         * @brief getName returns the function name
         * @return the function name
         */
        QString& getName(void);
        /**
         * @brief childsDeleteable returns wether childs are deleteable or not (sometimes subclasses of this class
         * shall not be able to delete childs, since that makes no sense in some cases, e.g. for integral nodes (fix
         * number of childs))
         * @return true if childs are deleteable, false otherwise
         */
        virtual bool childsDeleteable(void) override;

protected:

        QString m_name;
};

#endif // EGCFUNCTIONNODE_H
