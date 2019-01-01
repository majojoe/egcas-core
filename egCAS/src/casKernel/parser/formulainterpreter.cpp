
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
        m_iterPointer3(nullptr),
        m_parseKernelResult(false),
        m_isErrorOccurred(false)
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
        m_isErrorOccurred = false;

        ANTLRInputStream input(istream);
        EgcLexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        EgcParser parser(&tokens);
        parser.removeErrorListeners();
        parser.addErrorListener(this);

        EgcParser::FormulaContext* tree = parser.formula();

        antlrcpp::Any nodeTree;
        try {
                nodeTree = visitFormula(tree);
        } catch (std::runtime_error &e) {
                (void) e;
                return 1;
        } catch (std::out_of_range &e) {
                (void) e;
                return 1;
        }

        m_rootNode.reset(nodeTree);

        if (nodeTree.isNull())
            return 1;

        return 0;
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
    //cout << "formula: " << start << "/" << stop << endl;

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
        EgcNode *nodePtr = nullptr;
        QScopedPointer<EgcNumberNode> node(static_cast<EgcNumberNode*>(EgcNodeCreator::create(EgcNodeType::NumberNode)));
        if (node.isNull())
                return nullptr;
        node->setValue(QString::fromStdString(ctx->NUMBER()->getText()));
        nodePtr = node.data();
        addDanglingNode(node.take());

        return nodePtr;
}

antlrcpp::Any FormulaInterpreter::visitEquality(EgcParser::EqualityContext *ctx)
{
        return addBinaryExpression(EgcNodeType::EqualNode, visit(ctx->expr(0)), visit(ctx->expr(1)));
}

antlrcpp::Any FormulaInterpreter::visitDefinition(EgcParser::DefinitionContext *ctx)
{
        return addBinaryExpression(EgcNodeType::DefinitionNode, visit(ctx->expr(0)), visit(ctx->expr(1)));
}

antlrcpp::Any FormulaInterpreter::visitRedParenthesisL(EgcParser::RedParenthesisLContext *ctx)
{
        return addUnaryExpression(EgcNodeType::LParenthesisNode, visit(ctx->expr()));
}

antlrcpp::Any FormulaInterpreter::visitUMinus(EgcParser::UMinusContext *ctx)
{
        return addUnaryExpression(EgcNodeType::UnaryMinusNode, visit(ctx->expr()));
}

antlrcpp::Any FormulaInterpreter::visitVariable(EgcParser::VariableContext *ctx)
{
        EgcNode *nodePtr = nullptr;
        QScopedPointer<EgcVariableNode> node(static_cast<EgcVariableNode*>(EgcNodeCreator::create(EgcNodeType::VariableNode)));
        if (node.isNull())
                return nullptr;
        QString var = QString::fromStdString(ctx->NAMES()->getText());
        if (ctx->VARSUB())
                var += QString::fromStdString(ctx->VARSUB()->getText());
        node->setStuffedVar(var);
        nodePtr = node.data();
        addDanglingNode(node.take());

        return nodePtr;
}

antlrcpp::Any FormulaInterpreter::visitBracketOp(EgcParser::BracketOpContext *ctx)
{
        return visit(ctx->expr());
}

antlrcpp::Any FormulaInterpreter::visitDifferential(EgcParser::DifferentialContext *ctx)
{
        return addDifferentialExpression(visit(ctx->explist()));
}

antlrcpp::Any FormulaInterpreter::visitRoot(EgcParser::RootContext *ctx)
{
        return addBinaryExpression(EgcNodeType::RootNode, visit(ctx->expr(0)), visit(ctx->expr(1)));
}

antlrcpp::Any FormulaInterpreter::visitExponent(EgcParser::ExponentContext *ctx)
{
        return addBinaryExpression(EgcNodeType::ExponentNode, visit(ctx->expr(0)), visit(ctx->expr(1)));
}

antlrcpp::Any FormulaInterpreter::visitMulDiv(EgcParser::MulDivContext *ctx)
{
        EgcNode* retval = nullptr;
        size_t t = ctx->op->getType();
        if (t == EgcParser::MULT)
                retval = addBinaryExpression(EgcNodeType::MultiplicationNode, visit(ctx->expr(0)), visit(ctx->expr(1)));
        else
                retval = addDivisionExpression(visit(ctx->expr(0)), visit(ctx->expr(1)));

        return retval;
}

antlrcpp::Any FormulaInterpreter::visitSqrt(EgcParser::SqrtContext *ctx)
{
        return addSqrtExpression(visit(ctx->expr()));
}

antlrcpp::Any FormulaInterpreter::visitEmptyBinOp(EgcParser::EmptyBinOpContext *ctx)
{
        return addBinaryExpression(EgcNodeType::BinEmptyNode, visit(ctx->expr(0)), visit(ctx->expr(1)));
}

antlrcpp::Any FormulaInterpreter::visitIteratorL(EgcParser::IteratorLContext *ctx)
{
        return updateIterator(visit(ctx->expr()), 2);
}

antlrcpp::Any FormulaInterpreter::visitRedParenthesisR(EgcParser::RedParenthesisRContext *ctx)
{
        return addUnaryExpression(EgcNodeType::RParenthesisNode, visit(ctx->expr()));
}

antlrcpp::Any FormulaInterpreter::visitFunction(EgcParser::FunctionContext *ctx)
{
        std::string var = "";
        if (ctx->NAMES())
                var = ctx->NAMES()->getText();
        return addFunction(var, visit(ctx->explist()));
}

antlrcpp::Any FormulaInterpreter::visitParenthesis(EgcParser::ParenthesisContext *ctx)
{
        return addUnaryExpression(EgcNodeType::ParenthesisNode, visit(ctx->expr()));
}

antlrcpp::Any FormulaInterpreter::visitEmpty(EgcParser::EmptyContext *ctx)
{
        (void) ctx;
        return addEmptyNode();
}

antlrcpp::Any FormulaInterpreter::visitIteratorR(EgcParser::IteratorRContext *ctx)
{
        return updateIterator(visit(ctx->expr()), 1);
}

antlrcpp::Any FormulaInterpreter::visitNatlogarithm(EgcParser::NatlogarithmContext *ctx)
{
        return addUnaryExpression(EgcNodeType::NatLogNode, visit(ctx->expr()));
}

antlrcpp::Any FormulaInterpreter::visitLogarithm(EgcParser::LogarithmContext *ctx)
{
        return addUnaryExpression(EgcNodeType::LogNode, visit(ctx->expr()));
}

antlrcpp::Any FormulaInterpreter::visitIntegral(EgcParser::IntegralContext *ctx)
{
        return changeFlexExpressionType(EgcNodeType::IntegralNode, visit(ctx->explist()));
}

antlrcpp::Any FormulaInterpreter::visitCreateArglist(EgcParser::CreateArglistContext *ctx)
{
        return createArgList(visit(ctx->expr()));
}

antlrcpp::Any FormulaInterpreter::visitAddArgument(EgcParser::AddArgumentContext *ctx)
{
        return addArgument(visit(ctx->expr()), visit(ctx->explist()));
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

EgcNode* FormulaInterpreter::addUnaryExpression(EgcNodeType type, EgcNode* node0)
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

bool FormulaInterpreter::removeParenthesisChild(EgcNode& parenthesisNode)
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

void FormulaInterpreter::setRootNode(EgcNode* node)
{
        m_rootNode.reset(node);
        setNotDangling(node);
}

EgcNode* FormulaInterpreter::getRootNode(void)
{
        return m_rootNode.take();
}

EgcNode* FormulaInterpreter::isBuiltinOperation(const std::string& fncName, EgcNode* node)
{
        EgcNode* retval = nullptr;

        if (fncName == "log")
                retval = addUnaryExpression(EgcNodeType::NatLogNode, node);
        else if (fncName == "sqrt")
                retval = addSqrtExpression(node);

        return retval;
}

EgcNode* FormulaInterpreter::addFunction(const std::string& fncName, EgcNode* argList)
{
        EgcNode* builtinOp;
        EgcArgumentsNode* argL = static_cast<EgcArgumentsNode*>(argList);
        if (argList) {
                if (argL->getNumberChildNodes() == 1) {
                        builtinOp = isBuiltinOperation(fncName, static_cast<EgcNode*>(argList));
                        if (builtinOp)
                                return builtinOp;
                }
        }

        if (argList) {
                EgcNode* node = changeFlexExpressionType(EgcNodeType::FunctionNode, argList);
                EgcFunctionNode* function = static_cast<EgcFunctionNode*>(node);
                function->setStuffedName(QString::fromStdString(fncName));
                return static_cast<EgcNode*> (function);
        }

        return static_cast<EgcNode*> (argList);
}

EgcNode* FormulaInterpreter::updateIterator(EgcNode* node0, int i)
{
        if (i == 1)
                m_iterPointer1 = node0;
        if (i == 2)
                m_iterPointer2 = node0;
        if (i == 3)
                m_iterPointer3 = node0;

        return node0;
}

EgcNode* FormulaInterpreter::updateIterator(EgcNode* original, EgcNode* new_pointer)
{
        if (original == m_iterPointer1)
                m_iterPointer1 = new_pointer;
        if (original == m_iterPointer2)
                m_iterPointer2 = new_pointer;
        if (original == m_iterPointer3)
                m_iterPointer3 = new_pointer;

        return new_pointer;
}

EgcNode* FormulaInterpreter::createArgList(EgcNode* expression)
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

EgcNode* FormulaInterpreter::addArgument(EgcNode* expressionToAdd, EgcNode* argumentList)
{
        QScopedPointer<EgcNode> exprToAdd(expressionToAdd);
        setNotDangling(expressionToAdd);
        if (argumentList && expressionToAdd) {
                EgcArgumentsNode* argLst = static_cast<EgcArgumentsNode*>(argumentList);
                if (argLst->insert(0, *exprToAdd.data()))
                        (void) exprToAdd.take();
        }

        return argumentList;
}

void FormulaInterpreter::addDanglingNode(EgcNode* node)
{
        m_danglingNodes.insert(node);
}

void FormulaInterpreter::setNotDangling(EgcNode* node)
{
        m_danglingNodes.remove(node);
}

void FormulaInterpreter::deleteDanglingNodes(void)
{
        foreach (EgcNode* node, m_danglingNodes) {
                delete node;
        }
        m_danglingNodes.clear();
}

EgcNode* FormulaInterpreter::addDivisionExpression(EgcNode* node0, EgcNode* node1)
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

EgcNode* FormulaInterpreter::addEmptyNode(void)
{
        EgcNode *node = EgcNodeCreator::create(EgcNodeType::EmptyNode);
        if (node) {
                addDanglingNode(node);
        } else {
                throw std::runtime_error("Not enough memory to complete operation!");
        }

        return node;
}

EgcNode* FormulaInterpreter::changeFlexExpressionType(EgcNodeType type, EgcNode* argList)
{
        QScopedPointer<EgcFncContainerNode> node(static_cast<EgcFncContainerNode*>(EgcNodeCreator::create(type)));
        assert(node->isFlexNode());
        if (node->isFlexNode()) {
                if (argList) {
                        if (argList->getNodeType() == EgcNodeType::ArgumentsNode)
                                node->transferArgs(*static_cast<EgcArgumentsNode*>(argList));
                }
                delete argList;
                setNotDangling(argList);
                EgcFlexNode *nodePtr = node.data();
                addDanglingNode(node.take());

                return nodePtr;
        } else {
                return argList;
        }
}

EgcNode* FormulaInterpreter::addSqrtExpression(EgcNode* node0)
{
        QScopedPointer<EgcEmptyNode> nExp(static_cast<EgcEmptyNode*>(EgcNodeCreator::create(EgcNodeType::EmptyNode)));
        QScopedPointer<EgcNode> sqrt(addBinaryExpression(EgcNodeType::RootNode, nExp.take(), node0));

        return sqrt.take();
}

EgcNode* FormulaInterpreter::addDifferentialExpression(EgcNode* argList)
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

EgcNode* FormulaInterpreter::getIteratorNode(int i)
{
        if (i == 1)
                return m_iterPointer1;
        if (i == 2)
                return m_iterPointer2;
        if (i == 3)
                return m_iterPointer3;

        return nullptr;
}

void FormulaInterpreter::syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line, size_t charPositionInLine, const string &msg, std::exception_ptr e)
{
        (void) recognizer;
        (void) line;
        (void) e;

        m_isErrorOccurred = true;
        QString tmp;

        for (size_t i=0; i < charPositionInLine; i++)
                tmp += QString(" ");
        size_t start = offendingSymbol->getStartIndex();
        size_t stop = offendingSymbol->getStopIndex();
        for (size_t i= start; i <= stop; i++) {
                tmp += QString("^");
        }

        m_errorMessage = QString(msg.c_str() + QString('\n') + tmp);
}

QString FormulaInterpreter::getErrorMessage(void)
{
        return m_errorMessage;
}

bool FormulaInterpreter::isParsingErrorOccurred(void)
{
        return m_isErrorOccurred;
}
