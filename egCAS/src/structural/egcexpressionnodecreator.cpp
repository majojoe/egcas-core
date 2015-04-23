#include "egcexpressionnodecreator.h"
#include "egcnodes.h"

EgcExpressionNode* EgcExpressionNodeCreator::create(EgcExpressionNodeType type)
{
        EgcExpressionNode *retval;
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
                cog.outl("case EgcExpressionNodeType::%s:" % node[1])
                cog.outl("        retval = new (std::nothrow) %s();" % node[0])
                cog.outl("        break;")

        ]]]*/
        case EgcExpressionNodeType::EmptyNode:
                retval = new (std::nothrow) EgcEmptyExpressionNode();
                break;
        case EgcExpressionNodeType::BaseNode:
                retval = new (std::nothrow) EgcBaseExpressionNode();
                break;
        case EgcExpressionNodeType::NumberNode:
                retval = new (std::nothrow) EgcNumberExpressionNode();
                break;
        case EgcExpressionNodeType::VariableNode:
                retval = new (std::nothrow) EgcVariableExpressionNode();
                break;
        case EgcExpressionNodeType::ParenthesisNode:
                retval = new (std::nothrow) EgcParenthesisExpressionNode();
                break;
        case EgcExpressionNodeType::RootNode:
                retval = new (std::nothrow) EgcRootExpressionNode();
                break;
        //[[[end]]]
        //The list is generated automatically. Do NOT change it manually.

        default: //EgcExpressionNodeType::NodeUndefined
                qDebug("The implementation of this function is not complete, Please provide an implementation for the newly added EgcExpressionNodeType.");
                retval = nullptr;
                break;
        }

        return retval;
}
