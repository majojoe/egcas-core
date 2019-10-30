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

#include "egcnodecreator.h"
#include "egcnodes.h"

EgcNode* EgcNodeCreator::create(EgcNodeType type)
{
        EgcNode *retval;
        switch (type) {

        case EgcNodeType::DefinitionNode:
                retval = new (std::nothrow) EgcDefinitionNode();
                break;
        case EgcNodeType::FunctionNode:
                retval = new (std::nothrow) EgcFunctionNode();
                break;
        case EgcNodeType::NatLogNode:
                retval = new (std::nothrow) EgcNatLogNode();
                break;
        case EgcNodeType::RootNode:
                retval = new (std::nothrow) EgcRootNode();
                break;
        case EgcNodeType::EqualNode:
                retval = new (std::nothrow) EgcEqualNode();
                break;
        case EgcNodeType::AlnumNode:
                retval = new (std::nothrow) EgcAlnumNode();
                break;
        case EgcNodeType::NumberNode:
                retval = new (std::nothrow) EgcNumberNode();
                break;
        case EgcNodeType::MultiplicationNode:
                retval = new (std::nothrow) EgcMultiplicationNode();
                break;
        case EgcNodeType::MatrixNode:
                retval = new (std::nothrow) EgcMatrixNode();
                break;
        case EgcNodeType::ParenthesisNode:
                retval = new (std::nothrow) EgcParenthesisNode();
                break;
        case EgcNodeType::DivisionNode:
                retval = new (std::nothrow) EgcDivisionNode();
                break;
        case EgcNodeType::FncContainerNode:
                retval = new (std::nothrow) EgcFncContainerNode();
                break;
        case EgcNodeType::IntegralNode:
                retval = new (std::nothrow) EgcIntegralNode();
                break;
        case EgcNodeType::RParenthesisNode:
                retval = new (std::nothrow) RParenthesisNode();
                break;
        case EgcNodeType::DifferentialNode:
                retval = new (std::nothrow) EgcDifferentialNode();
                break;
        case EgcNodeType::LParenthesisNode:
                retval = new (std::nothrow) LParenthesisNode();
                break;
        case EgcNodeType::PlusNode:
                retval = new (std::nothrow) EgcPlusNode();
                break;
        case EgcNodeType::ConstantNode:
                retval = new (std::nothrow) EgcConstantNode();
                break;
        case EgcNodeType::UnaryMinusNode:
                retval = new (std::nothrow) EgcUnaryMinusNode();
                break;
        case EgcNodeType::VariableNode:
                retval = new (std::nothrow) EgcVariableNode();
                break;
        case EgcNodeType::MinusNode:
                retval = new (std::nothrow) EgcMinusNode();
                break;
        case EgcNodeType::BinEmptyNode:
                retval = new (std::nothrow) EgcBinEmptyNode();
                break;
        case EgcNodeType::ExponentNode:
                retval = new (std::nothrow) EgcExponentNode();
                break;
        case EgcNodeType::LogNode:
                retval = new (std::nothrow) EgcLogNode();
                break;
        case EgcNodeType::ArgumentsNode:
                retval = new (std::nothrow) EgcArgumentsNode();
                break;
        case EgcNodeType::EmptyNode:
                retval = new (std::nothrow) EgcEmptyNode();
                break;
        //The list is generated automatically. Do NOT change it manually.

        case EgcNodeType::BaseNode:
                retval = new (std::nothrow) EgcBaseNode();
                break;
        default: //EgcNodeType::NodeUndefined
                qDebug("The implementation of this function is not complete, Please provide an implementation for the newly added EgcNodeType.");
                retval = nullptr;
                break;
        }

        return retval;
}

EgcNode*EgcNodeCreator::create(QLatin1String name)
{
        EgcNode *retval = nullptr;

        if (name == QLatin1String("definitionnode"))
                retval = new (std::nothrow) EgcDefinitionNode();
        if (name == QLatin1String("functionnode"))
                retval = new (std::nothrow) EgcFunctionNode();
        if (name == QLatin1String("natlognode"))
                retval = new (std::nothrow) EgcNatLogNode();
        if (name == QLatin1String("rootnode"))
                retval = new (std::nothrow) EgcRootNode();
        if (name == QLatin1String("equalnode"))
                retval = new (std::nothrow) EgcEqualNode();
        if (name == QLatin1String("alnumnode"))
                retval = new (std::nothrow) EgcAlnumNode();
        if (name == QLatin1String("numbernode"))
                retval = new (std::nothrow) EgcNumberNode();
        if (name == QLatin1String("multiplicationnode"))
                retval = new (std::nothrow) EgcMultiplicationNode();
        if (name == QLatin1String("matrixnode"))
                retval = new (std::nothrow) EgcMatrixNode();
        if (name == QLatin1String("parenthesisnode"))
                retval = new (std::nothrow) EgcParenthesisNode();
        if (name == QLatin1String("divisionnode"))
                retval = new (std::nothrow) EgcDivisionNode();
        if (name == QLatin1String("fnccontainernode"))
                retval = new (std::nothrow) EgcFncContainerNode();
        if (name == QLatin1String("integralnode"))
                retval = new (std::nothrow) EgcIntegralNode();
        if (name == QLatin1String("rparenthesisnode"))
                retval = new (std::nothrow) RParenthesisNode();
        if (name == QLatin1String("differentialnode"))
                retval = new (std::nothrow) EgcDifferentialNode();
        if (name == QLatin1String("lparenthesisnode"))
                retval = new (std::nothrow) LParenthesisNode();
        if (name == QLatin1String("plusnode"))
                retval = new (std::nothrow) EgcPlusNode();
        if (name == QLatin1String("constantnode"))
                retval = new (std::nothrow) EgcConstantNode();
        if (name == QLatin1String("unaryminusnode"))
                retval = new (std::nothrow) EgcUnaryMinusNode();
        if (name == QLatin1String("variablenode"))
                retval = new (std::nothrow) EgcVariableNode();
        if (name == QLatin1String("minusnode"))
                retval = new (std::nothrow) EgcMinusNode();
        if (name == QLatin1String("binemptynode"))
                retval = new (std::nothrow) EgcBinEmptyNode();
        if (name == QLatin1String("exponentnode"))
                retval = new (std::nothrow) EgcExponentNode();
        if (name == QLatin1String("lognode"))
                retval = new (std::nothrow) EgcLogNode();
        if (name == QLatin1String("argumentsnode"))
                retval = new (std::nothrow) EgcArgumentsNode();
        if (name == QLatin1String("emptynode"))
                retval = new (std::nothrow) EgcEmptyNode();
        //The list is generated automatically. Do NOT change it manually.

        if (name == QLatin1String("basenode"))
                retval = new (std::nothrow) EgcBaseNode();


        return retval;
}

QLatin1String EgcNodeCreator::stringize(EgcNodeType type)
{
        QLatin1String retval = QLatin1String("");

        switch (type) {

        case EgcNodeType::DefinitionNode:
                retval = QLatin1String("definitionnode");
                break;
        case EgcNodeType::FunctionNode:
                retval = QLatin1String("functionnode");
                break;
        case EgcNodeType::NatLogNode:
                retval = QLatin1String("natlognode");
                break;
        case EgcNodeType::RootNode:
                retval = QLatin1String("rootnode");
                break;
        case EgcNodeType::EqualNode:
                retval = QLatin1String("equalnode");
                break;
        case EgcNodeType::AlnumNode:
                retval = QLatin1String("alnumnode");
                break;
        case EgcNodeType::NumberNode:
                retval = QLatin1String("numbernode");
                break;
        case EgcNodeType::MultiplicationNode:
                retval = QLatin1String("multiplicationnode");
                break;
        case EgcNodeType::MatrixNode:
                retval = QLatin1String("matrixnode");
                break;
        case EgcNodeType::ParenthesisNode:
                retval = QLatin1String("parenthesisnode");
                break;
        case EgcNodeType::DivisionNode:
                retval = QLatin1String("divisionnode");
                break;
        case EgcNodeType::FncContainerNode:
                retval = QLatin1String("fnccontainernode");
                break;
        case EgcNodeType::IntegralNode:
                retval = QLatin1String("integralnode");
                break;
        case EgcNodeType::RParenthesisNode:
                retval = QLatin1String("rparenthesisnode");
                break;
        case EgcNodeType::DifferentialNode:
                retval = QLatin1String("differentialnode");
                break;
        case EgcNodeType::LParenthesisNode:
                retval = QLatin1String("lparenthesisnode");
                break;
        case EgcNodeType::PlusNode:
                retval = QLatin1String("plusnode");
                break;
        case EgcNodeType::ConstantNode:
                retval = QLatin1String("constantnode");
                break;
        case EgcNodeType::UnaryMinusNode:
                retval = QLatin1String("unaryminusnode");
                break;
        case EgcNodeType::VariableNode:
                retval = QLatin1String("variablenode");
                break;
        case EgcNodeType::MinusNode:
                retval = QLatin1String("minusnode");
                break;
        case EgcNodeType::BinEmptyNode:
                retval = QLatin1String("binemptynode");
                break;
        case EgcNodeType::ExponentNode:
                retval = QLatin1String("exponentnode");
                break;
        case EgcNodeType::LogNode:
                retval = QLatin1String("lognode");
                break;
        case EgcNodeType::ArgumentsNode:
                retval = QLatin1String("argumentsnode");
                break;
        case EgcNodeType::EmptyNode:
                retval = QLatin1String("emptynode");
                break;
        //The list is generated automatically. Do NOT change it manually.

        case EgcNodeType::BaseNode:
                retval = QLatin1String("basenode");
                break;
        }

        return retval;

}
