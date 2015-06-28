#ifndef EGCNODETYPE_GEN_H
#define EGCNODETYPE_GEN_H


/**
 * @brief The EgcNodeType enum is a enum to differentiate the different node types
 * 
 * This list is generated form the cog python code below. Do not change it manually!!!
 */
enum class EgcNodeType
{
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

#NumberNode = 0, VariableNode, RootNode, ParenthesisNode, BaseNode, EmptyNode, NodeUndefined
first = nodes[0][1]
for node in nodes:
    if (node[1] != "NodeUndefined") and (node[1] != "BaseNode"):
        if node[1] is first:
            cog.outl("        %s = 0," % node[1])
        else:
            cog.outl("        %s," % node[1])
cog.outl("        BaseNode,")
cog.outl("        NodeUndefined")
]]]*/
EmptyNode = 0,
ParenthesisNode,
VariableNode,
FunctionNode,
NumberNode,
RootNode,
BaseNode,
NodeUndefined
//[[[end]]]
//The list is generated automatically. Do NOT change it manually.
};

#endif //#ifndef EGCNODETYPE_GEN_H
