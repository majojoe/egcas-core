/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#ifndef EGCVARIABLENODE_H
#define EGCVARIABLENODE_H

#include <QString>
#include "../specialNodes/egcnode.h"

/**
 * @brief The EgcVariableNode class is a class that holds the leafes with variable names
 */
class EgcVariableNode : public EgcNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcVariableNode, EgcNodeType::VariableNode);

public:
        ///std constructor
        EgcVariableNode();
        ///destructor
        virtual ~EgcVariableNode();
        /**
         * @brief setValue set the variable name (value)
         * @param varName the variable name as a string
         * @param subscript the subscript of the variable if any
         */
        virtual void setValue(const QString& varName, const QString& subscript);
        /**
         * @brief setValue set the raw variable name (value) maybe including stuffed subscript
         * @param varName the variable name as a string. This can include the stuffed subscript
         */
        virtual void setValueRaw(const QString& varName);
        /**
         * @brief getValue returns the variable name (without subscript)
         * @return the variable name
         */
        QString& getValue(void);
        /**
         * @brief getSubscript returns the subscript of a variable name
         * @return the subscript of this variable object
         */
        virtual QString& getSubscript(void);
        /**
         * @brief getStuffedVar returns the stuffed variable name (with subscript)
         * @return the stuffed variable name (a "_" in the variable name is stuffed into "__",
         * and variable name and subscript is seperated via "_")
         */
        virtual QString getStuffedVar(void);
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * A variable expression is valid if the value is not empty.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief isEqual checks if this node (and its childs) is equal with the node given (and those childs)
         * @return true if the node given is equal with this node, false otherwise.
         */
        virtual bool isEqual(EgcNode* node) const override;

protected:
        QString m_value;          ///< the variable name used
        QString m_subscript;      ///< the subscript if any        
};

#endif // EGCVARIABLENODE_H
