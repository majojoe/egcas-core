/*Copyright (c) 2016, Johannes Maier <maier_jo@gmx.de>
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

#include <QString>
#include <QtTest>
#include <QMap>
#include <iostream>
#include <iomanip>
#include "structural/egcnodes.h"
#include "structural/specialNodes/egccontainernode.h"


using namespace std;

class EgcasPrintOpPrec : public QObject
{
        Q_OBJECT

public:
        EgcasPrintOpPrec() {}
private Q_SLOTS:
        void printPrecedence();
private:
        void createList(void);

        QMap<qint32, QString> m_list;
};

void EgcasPrintOpPrec::createList(void)
{
        EgcNode* obj = nullptr;
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
                cog.outl("obj = new (std::nothrow) %s();" % node[0])
                cog.outl("if (obj->isContainer())")
                cog.outl("        m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), \"%s\");" % node[1])
                cog.outl("delete(obj);")

        ]]]*/
        obj = new (std::nothrow) EgcEmptyNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "EmptyNode");
        delete(obj);
        obj = new (std::nothrow) EgcArgumentsNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "ArgumentsNode");
        delete(obj);
        obj = new (std::nothrow) EgcReorderingProtectorNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "ReorderingProtector");
        delete(obj);
        obj = new (std::nothrow) EgcAlnumNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "AlnumNode");
        delete(obj);
        obj = new (std::nothrow) EgcMultiplicationNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "MultiplicationNode");
        delete(obj);
        obj = new (std::nothrow) EgcVariableNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "VariableNode");
        delete(obj);
        obj = new (std::nothrow) EgcFunctionNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "FunctionNode");
        delete(obj);
        obj = new (std::nothrow) EgcDivisionNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "DivisionNode");
        delete(obj);
        obj = new (std::nothrow) EgcNumberNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "NumberNode");
        delete(obj);
        obj = new (std::nothrow) EgcMinusNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "MinusNode");
        delete(obj);
        obj = new (std::nothrow) EgcIntegralNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "IntegralNode");
        delete(obj);
        obj = new (std::nothrow) EgcDefinitionNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "DefinitionNode");
        delete(obj);
        obj = new (std::nothrow) EgcExponentNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "ExponentNode");
        delete(obj);
        obj = new (std::nothrow) EgcRootNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "RootNode");
        delete(obj);
        obj = new (std::nothrow) EgcBinEmptyNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "BinEmptyNode");
        delete(obj);
        obj = new (std::nothrow) EgcParenthesisNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "ParenthesisNode");
        delete(obj);
        obj = new (std::nothrow) EgcDifferentialNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "DifferentialNode");
        delete(obj);
        obj = new (std::nothrow) EgcPlusNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "PlusNode");
        delete(obj);
        obj = new (std::nothrow) EgcUnaryMinusNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "UnaryMinusNode");
        delete(obj);
        obj = new (std::nothrow) EgcEqualNode();
        if (obj->isContainer())
                m_list.insertMulti(static_cast<EgcContainerNode*>(obj)->bindingPower(), "EqualNode");
        delete(obj);
        //[[[end]]]
        //The list is generated automatically. Do NOT change it manually.
}

void EgcasPrintOpPrec::printPrecedence()
{                
        createList();

        QMapIterator<qint32, QString> i(m_list);
        cout << endl << "operator precedence:" << endl;
        cout << "============================" << endl;
        while (i.hasNext()) {
                i.next();
                cout << setw(20) << i.value().toUtf8().data() << ":\t " << i.key() << endl;
        }
}

QTEST_MAIN(EgcasPrintOpPrec)

#include "tst_print_op_precedence.moc"
