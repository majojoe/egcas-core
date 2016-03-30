#ifndef EGCNODES_H
#define EGCNODES_H

#include "specialNodes/egcnode.h"
#include "specialNodes/egcunarynode.h"
#include "specialNodes/egcbinarynode.h"
#include "specialNodes/egcbasenode.h"
#include "specialNodes/egcemptynode.h"
#include "specialNodes/egcargumentsnode.h"

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

nodes = find_nodes_in_dir(BaseDirToSearch, "structural/concreteNodes")
nodes = flatten(nodes)

#NumberNode = 0, VariableNode, RootNode, ParenthesisNode, BaseNode, EmptyNode, NodeUndefined
first = nodes[0][1]
for node in nodes:
    cog.outl("#include \"concreteNodes/egc%s.h\"" % node[1].lower())

]]]*/
#include "concreteNodes/egcnumbernode.h"
#include "concreteNodes/egcunaryminusnode.h"
#include "concreteNodes/egcequalnode.h"
#include "concreteNodes/egcdefinitionnode.h"
#include "concreteNodes/egcmultiplicationnode.h"
#include "concreteNodes/egcvariablenode.h"
#include "concreteNodes/egcrootnode.h"
#include "concreteNodes/egcdivisionnode.h"
#include "concreteNodes/egcexponentnode.h"
#include "concreteNodes/egcalnumnode.h"
#include "concreteNodes/egcfunctionnode.h"
#include "concreteNodes/egcplusnode.h"
#include "concreteNodes/egcintegralnode.h"
#include "concreteNodes/egcparenthesisnode.h"
#include "concreteNodes/egcdifferentialnode.h"
#include "concreteNodes/egcminusnode.h"
//[[[end]]]
//The list is generated automatically. Do NOT change it manually.

#endif // EGCNODES_H
