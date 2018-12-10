
#include <sstream>
#include <QVector>
#include <QScopedPointer>
#include <QString>
#include <antlr4-runtime/antlr4-runtime.h>
#include "formulainterpreter.h"
#include "EgcLexer.h"
#include "EgcParser.h"
#include "../../structural/egcnodecreator.h"
#include "../../structural/egcnodes.h"
#include "../../structural/specialNodes/egcnode.h"


using namespace std;
using namespace antlrcpp;
using namespace antlr4;



FormulaInterpreter::FormulaInterpreter() :
        m_location(0),
        m_iterPointer1(nullptr),
        m_iterPointer2(nullptr),
        m_iterPointer3(nullptr)
{

}

FormulaInterpreter::~FormulaInterpreter()
{
        deleteDanglingNodes();
}

int FormulaInterpreter::parse(std::istream &istream, bool parseKernelResult)
{
        m_parseKernelResult = parseKernelResult;
        m_iterPointer1 = nullptr;
        m_iterPointer2 = nullptr;
        m_iterPointer3 = nullptr;
        m_location = 0;

        ANTLRInputStream input(istream);
        EgcLexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        EgcParser parser(&tokens);

        EgcParser::FormulaContext* tree = parser.formula();

        antlrcpp::Any nodeTree;
        try {
                nodeTree = visitFormula(tree);
        } catch (std::runtime_error &e) {
                (void) e;
                return 1;
        }

        m_rootNode.reset(nodeTree);

        if (nodeTree)
            return 0;

        return 1;
}

void FormulaInterpreter::clear() {
        m_location = 0;
        m_rootNode.reset(nullptr);
        deleteDanglingNodes();
#if (EGC_PARSER_DEBUG >= 3)
#endif //#if (EGC_PARSER_DEBUG >= 3)
}

antlrcpp::Any FormulaInterpreter::visitFormula(EgcParser::FormulaContext *ctx)
{
    size_t start = ctx->getStart()->getCharPositionInLine();
    Token* stopToken = ctx->getStop();
    size_t stop = stopToken->getCharPositionInLine();
    stop += stopToken->getText().length() - 1;
    cout << "formula: " << start << "/" << stop << endl;

    Any retval = visitChildren(ctx);
    setNotDangling(retval);

    return retval;
}

antlrcpp::Any FormulaInterpreter::visitPlusMinus(EgcParser::PlusMinusContext *ctx)
{
        EgcNode* retval = nullptr;
        size_t t = ctx->op->getType();
        if (t == EgcParser::PLUS)
                retval = addBinaryExpression(EgcNodeType::PlusNode, visit(ctx->expr(0)), visit(ctx->expr(1)));
        else
                retval = addBinaryExpression(EgcNodeType::MinusNode, visit(ctx->expr(0)), visit(ctx->expr(1)));

        return retval;
}

antlrcpp::Any FormulaInterpreter::visitNumber(EgcParser::NumberContext *ctx)
{
    size_t start = ctx->getStart()->getCharPositionInLine();
    Token* stopToken = ctx->getStop();
    size_t stop = stopToken->getCharPositionInLine();
    stop += stopToken->getText().length() - 1;

    cout << "number: " << start << "/" << stop << endl;

  return visitChildren(ctx);
}

EgcNode* FormulaInterpreter::addBinaryExpression(EgcNodeType type, EgcNode* node0,
                                                    EgcNode* node1)
{
        QScopedPointer<EgcBinaryNode> node;
        node.reset(static_cast<EgcBinaryNode*>(EgcNodeCreator::create(type)));
        QScopedPointer<EgcNode> node0Tmp(node0);
        QScopedPointer<EgcNode> node1Tmp(node1);
        setNotDangling(node0);
        setNotDangling(node1);
        if (!node.isNull() && node0 && node1) {
                node->setChild(0, *node0Tmp.take());
                node->setChild(1, *node1Tmp.take());
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }
        EgcNode *nodePtr = node.data();
        addDanglingNode(node.take());

        return nodePtr;
}

EgcNode* Interpreter::addUnaryExpression(EgcNodeType type, EgcNode* node0)
{
        QScopedPointer<EgcUnaryNode> node(static_cast<EgcUnaryNode*>(EgcNodeCreator::create(type)));
        QScopedPointer<EgcNode> node0Tmp(node0);
        setNotDangling(node0);
        if (!node.isNull() && node0) {
                node->setChild(0, *node0Tmp.take());
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }
        EgcNode *nodePtr = node.data();
        addDanglingNode(node.take());

        return nodePtr;
}

bool Interpreter::removeParenthesisChild(EgcNode& parenthesisNode)
{
        if (parenthesisNode.getNodeType() != EgcNodeType::ParenthesisNode)
                return false;
        EgcContainerNode &pNode = static_cast<EgcContainerNode&>(parenthesisNode);

        EgcContainerNode *parent = parenthesisNode.getParent();
        if (!parent)
                return false;
        quint32 i;
        if (!parent->getIndexOfChild(parenthesisNode, i))
                return false;
        EgcNode* child;
        QScopedPointer<EgcNode> tempChild;
        child = pNode.getChild(0);
        if (child) {
                tempChild.reset(pNode.takeOwnership(*child));
                parent->setChild(i, *tempChild.take());
        }
        parent->setChild(i, *child);

        return true;
}

EgcNode* Interpreter::addStringNode(EgcNodeType type, const std::string& value)
{
        EgcNode *nodePtr = nullptr;
        QScopedPointer<EgcNode> node(static_cast<EgcNode*>(EgcNodeCreator::create(type)));
        if (!node.isNull()) {
                switch (type) {
                case EgcNodeType::NumberNode: {
                        EgcNumberNode* tmp = static_cast<EgcNumberNode*>(node.data());
                        tmp->setValue(QString::fromStdString(value));
                        break;
                }
                case EgcNodeType::VariableNode: {
                        EgcVariableNode* tmp = static_cast<EgcVariableNode*>(node.data());
                        tmp->setStuffedVar(QString::fromStdString(value));
                        break;
                }
                default: /*do nothing*/
                        break;
                }
                nodePtr = node.data();
                addDanglingNode(node.take());
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }


        return nodePtr;
}

void Interpreter::setRootNode(EgcNode* node)
{
        m_rootNode.reset(node);
        setNotDangling(node);
}

EgcNode* Interpreter::getRootNode(void)
{
        return m_rootNode.take();
}

EgcNode* Interpreter::isBuiltinOperation(const std::string& fncName, EgcNode* node)
{
        EgcNode* retval = nullptr;

        if (fncName == "log")
                retval = addUnaryExpression(EgcNodeType::NatLogNode, node);
        else if (fncName == "sqrt")
                retval = addSqrtExpression(node);

        return retval;
}

EgcNode* Interpreter::addFunction(const std::string& fncName, EgcArgumentsNode* argList)
{

        EgcNode* builtinOp;
        if (argList->getNumberChildNodes() == 1) {
                builtinOp = isBuiltinOperation(fncName, static_cast<EgcNode*>(argList));
                if (builtinOp)
                        return builtinOp;
        }

        if (argList) {
                EgcNode* node = changeFlexExpressionType(EgcNodeType::FunctionNode, argList);
                EgcFunctionNode* function = static_cast<EgcFunctionNode*>(node);
                function->setStuffedName(QString::fromStdString(fncName));
                return static_cast<EgcNode*> (function);
        }

        return static_cast<EgcNode*> (argList);
}

EgcNode* Interpreter::updateIterator(EgcNode* node0, int i)
{
        if (i == 1)
                m_iterPointer1 = node0;
        if (i == 2)
                m_iterPointer2 = node0;
        if (i == 3)
                m_iterPointer3 = node0;

        return node0;
}

EgcNode* Interpreter::updateIterator(EgcNode* original, EgcNode* new_pointer)
{
        if (original == m_iterPointer1)
                m_iterPointer1 = new_pointer;
        if (original == m_iterPointer2)
                m_iterPointer2 = new_pointer;
        if (original == m_iterPointer3)
                m_iterPointer3 = new_pointer;

        return new_pointer;
}

EgcArgumentsNode* Interpreter::createArgList(EgcNode* expression)
{
        QScopedPointer<EgcArgumentsNode> node(static_cast<EgcArgumentsNode*>(EgcNodeCreator::create(EgcNodeType::ArgumentsNode)));
        QScopedPointer<EgcNode> exprPtr(expression);
        setNotDangling(expression);
        if (!node.isNull()) {
                node->setChild(0, *exprPtr.take());
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }
        EgcArgumentsNode *nodePtr = node.data();
        addDanglingNode(node.take());

        return nodePtr;
}

EgcArgumentsNode* Interpreter::addArgument(EgcNode* expressionToAdd, EgcArgumentsNode* argumentList)
{
        QScopedPointer<EgcNode> exprToAdd(expressionToAdd);
        setNotDangling(expressionToAdd);
        if (argumentList && expressionToAdd) {
                if (argumentList->insert(0, *exprToAdd.data()))
                        (void) exprToAdd.take();
        }

        return argumentList;
}

void Interpreter::addDanglingNode(EgcNode* node)
{
        m_danglingNodes.insert(node);
}

void Interpreter::setNotDangling(EgcNode* node)
{
        m_danglingNodes.remove(node);
}

void Interpreter::deleteDanglingNodes(void)
{
        foreach (EgcNode* node, m_danglingNodes) {
                delete node;
        }
        m_danglingNodes.clear();
}

EgcNode* Interpreter::addSqrtExpression(EgcNode* node0)
{
        QScopedPointer<EgcEmptyNode> nExp(static_cast<EgcEmptyNode*>(EgcNodeCreator::create(EgcNodeType::EmptyNode)));
        QScopedPointer<EgcNode> sqrt(addBinaryExpression(EgcNodeType::RootNode, nExp.take(), node0));
        
        return sqrt.take();        
}

EgcNode* Interpreter::addUnaryStructParenth(EgcNode* node)
{
        return node;
}

EgcNode* Interpreter::addDivisionExpression(EgcNode* node0, EgcNode* node1)
{
        EgcNode* retval;

        if (!m_parseKernelResult) {
                retval = addBinaryExpression(EgcNodeType::DivisionNode, node0, node1);
        } else {
                retval = addBinaryExpression(EgcNodeType::DivisionNode, node0, node1);
                if (node0) {
                        if (node0->getNodeType() == EgcNodeType::ParenthesisNode) {
                                (void) removeParenthesisChild(*node0);
                        }
                }
                if (node1) {
                        if (node1->getNodeType() == EgcNodeType::ParenthesisNode) {
                                (void) removeParenthesisChild(*node1);
                        }
                }
        }

        return retval;
}

EgcNode* Interpreter::addEmptyNode(void)
{
        EgcNode *node = EgcNodeCreator::create(EgcNodeType::EmptyNode);
        if (node) {
                addDanglingNode(node);
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }

        return node;
}

EgcNode* Interpreter::changeFlexExpressionType(EgcNodeType type, EgcArgumentsNode* argList)
{
        QScopedPointer<EgcFncContainerNode> node(static_cast<EgcFncContainerNode*>(EgcNodeCreator::create(type)));
        assert(node->isFlexNode());
        if (node->isFlexNode()) {
                if (argList)
                        node->transferArgs(*argList);
                delete argList;
                setNotDangling(argList);
                EgcFlexNode *nodePtr = node.data();
                addDanglingNode(node.take());

                return nodePtr;
        } else {
                return argList;
        }
}

EgcNode* Interpreter::addDifferentialExpression(EgcArgumentsNode* argList)
{
        EgcNode* node = changeFlexExpressionType(EgcNodeType::DifferentialNode, argList);
        EgcDifferentialNode* diff = static_cast<EgcDifferentialNode*>(node);

        unsigned int diff_type = 0;
        EgcNode* type = diff->getChild(0);
        if (!type)
                return node;
        if (type->getNodeType() == EgcNodeType::NumberNode)
                diff_type = static_cast<EgcNumberNode*>(type)->getValue().toUInt();
        diff->remove(0);
        if (diff_type == 1 || diff_type == 2 || diff_type == 3) {
                diff->setNrDerivative(static_cast<quint8>(diff_type));
                if (diff_type == 1) {
                        diff->setDifferentialType(EgcDifferentialNode::DifferentialType::lagrange1);
                        diff->setNrDerivative(1);
                } else if (diff_type == 2) {
                        diff->setDifferentialType(EgcDifferentialNode::DifferentialType::lagrange2);
                        diff->setNrDerivative(2);
                } else if (diff_type == 3) {
                        diff->setDifferentialType(EgcDifferentialNode::DifferentialType::lagrange3);
                        diff->setNrDerivative(3);
                }
        } else {
                EgcNode* derivative = diff->getChild(2);
                if (!derivative)
                        return node;
                unsigned int der = 1;
                if (derivative->getNodeType() == EgcNodeType::NumberNode)
                        der = static_cast<EgcNumberNode*>(derivative)->getValue().toUInt();
                if (der <= 0)
                        der = 1;
                diff->setNrDerivative(static_cast<quint8>(der));
                diff->setDifferentialType(EgcDifferentialNode::DifferentialType::leibnitz);
                if (der == 1) {
                        QScopedPointer<EgcEmptyNode> emptyNd(static_cast<EgcEmptyNode*>(EgcNodeCreator::create(EgcNodeType::EmptyNode)));
                        (void) updateIterator(diff->getChild(2), emptyNd.data());
                        diff->remove(2);
                        diff->insert(2, *emptyNd.take());
                }
        }

        return node;
}

EgcNode* Interpreter::getIteratorNode(int i)
{
        if (i == 1)
                return m_iterPointer1;
        if (i == 2)
                return m_iterPointer2;
        if (i == 3)
                return m_iterPointer3;

        return nullptr;
}
