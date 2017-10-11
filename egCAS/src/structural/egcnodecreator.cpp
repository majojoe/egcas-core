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

        /*[[[cog
        import cog
        import os
        import re

        def find_nodes_in_file(filename): 
            nodes = list()
            regex = re.compile("[\s]*EGC_SET_EXPRESSION_TYPE\(([a-zA-Z0-9_-]+)[\s]*,[\s]*[a-zA-Z0-9]+::([a-zA-Z0-9]+)[\s]*\)[\s]*;[\s]*", re.MULTILINE)
            with open(filename, "r") as f:
                content = f.read();
                result = regex.findall(content)
                if result:
                    nodes.append(result)
                else:
                    nodes = None
                f.close()
                return nodes
            return None

        def find_nodes_in_dir(base_path, sub_path):
            nodes = list()
            dir_complete = os.path.join(base_path, sub_path)
            files = list()
            for root, directories, filenames in os.walk(dir_complete):
                for filename in filenames: 
                    files.append(os.path.join(root,filename))
            for file in files:
                if file.endswith(".h") or file.endswith(".H"):
                    nodes_temp = find_nodes_in_file(file)
                    if nodes_temp:
                        nodes.append(nodes_temp)
            if not nodes:
                cog.error("Seems you have changed the macro name \"EGC_SET_EXPRESSION_TYPE\" please change the above regex also to function again!")
            return nodes

        def flatten(param):
            if not param: 
                return []
            elif type(param) is not list: 
                return [param]
            else:
                first_el = flatten(param[0])
                el = flatten(param[1:])
                first_el.extend(el)
                return first_el
            
        nodes = find_nodes_in_dir(BaseDirToSearch, "structural")
        nodes = flatten(nodes)

        for node in nodes:
            if node[1] != "NodeUndefined":
                cog.outl("case EgcNodeType::%s:" % node[1])
                cog.outl("        retval = new (std::nothrow) %s();" % node[0])
                cog.outl("        break;")

        ]]]*/
        case EgcNodeType::EmptyNode:
                retval = new (std::nothrow) EgcEmptyNode();
                break;
        case EgcNodeType::ArgumentsNode:
                retval = new (std::nothrow) EgcArgumentsNode();
                break;
        case EgcNodeType::NatLogNode:
                retval = new (std::nothrow) EgcNatLogNode();
                break;
        case EgcNodeType::AlnumNode:
                retval = new (std::nothrow) EgcAlnumNode();
                break;
        case EgcNodeType::MultiplicationNode:
                retval = new (std::nothrow) EgcMultiplicationNode();
                break;
        case EgcNodeType::VariableNode:
                retval = new (std::nothrow) EgcVariableNode();
                break;
        case EgcNodeType::FunctionNode:
                retval = new (std::nothrow) EgcFunctionNode();
                break;
        case EgcNodeType::DivisionNode:
                retval = new (std::nothrow) EgcDivisionNode();
                break;
        case EgcNodeType::LParenthesisNode:
                retval = new (std::nothrow) LParenthesisNode();
                break;
        case EgcNodeType::NumberNode:
                retval = new (std::nothrow) EgcNumberNode();
                break;
        case EgcNodeType::MinusNode:
                retval = new (std::nothrow) EgcMinusNode();
                break;
        case EgcNodeType::IntegralNode:
                retval = new (std::nothrow) EgcIntegralNode();
                break;
        case EgcNodeType::DefinitionNode:
                retval = new (std::nothrow) EgcDefinitionNode();
                break;
        case EgcNodeType::ExponentNode:
                retval = new (std::nothrow) EgcExponentNode();
                break;
        case EgcNodeType::LogNode:
                retval = new (std::nothrow) EgcLogNode();
                break;
        case EgcNodeType::RootNode:
                retval = new (std::nothrow) EgcRootNode();
                break;
        case EgcNodeType::BinEmptyNode:
                retval = new (std::nothrow) EgcBinEmptyNode();
                break;
        case EgcNodeType::FncContainerNode:
                retval = new (std::nothrow) EgcFncContainerNode();
                break;
        case EgcNodeType::ParenthesisNode:
                retval = new (std::nothrow) EgcParenthesisNode();
                break;
        case EgcNodeType::DifferentialNode:
                retval = new (std::nothrow) EgcDifferentialNode();
                break;
        case EgcNodeType::PlusNode:
                retval = new (std::nothrow) EgcPlusNode();
                break;
        case EgcNodeType::UnaryMinusNode:
                retval = new (std::nothrow) EgcUnaryMinusNode();
                break;
        case EgcNodeType::EqualNode:
                retval = new (std::nothrow) EgcEqualNode();
                break;
        case EgcNodeType::RParenthesisNode:
                retval = new (std::nothrow) RParenthesisNode();
                break;
        //[[[end]]]
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
