
#ifndef FORMULAINTERPRETER_H
#define FORMULAINTERPRETER_H

#include <QSet>
#include <QScopedPointer>
#include <antlr4-runtime/antlr4-runtime.h>
#include <antlr4-runtime/BaseErrorListener.h>
#include "EgcBaseVisitor.h"


class EgcNode;
enum class EgcNodeType;

class FormulaInterpreter : public EgcBaseVisitor, public antlr4::BaseErrorListener
{
public:
        FormulaInterpreter();
        ///destructor
        virtual ~FormulaInterpreter() override;

        /**
         * Run parser. Results are stored inside.
         * @param istream input stream to use for getting tokens
         * @param postProcessFormula if true some post processing takes place, e.g. removing some parenthesis where not
         * neccessary.
         * @return 0 on success, 1 on failure
         */
        int parse(std::istream &istream, bool parseKernelResult = false);

        /**
         * Clear AST of formula
         */
        void clear();

        /**
         * @brief getRootNode returns the root node of the lastly parsed formula. ATTENTION: the caller takes ownership
         * of the root node returned.
         * @return a pointer to the root node of the lastly parsed formula
         */
        EgcNode* getRootNode(void);

        /**
         * @brief getIteratorNode after parsing the iterator of the formula must be updated if there was one.
         * @param i iterator pointer to get
         * @return the node the iterator should point to after the parsing.
         */
        EgcNode* getIteratorNode(int i);

        void syntaxError(antlr4::Recognizer * recognizer, antlr4::Token * offendingSymbol, size_t line, size_t charPositionInLine,
                         const std::string &msg, std::exception_ptr e) override;

        /**
         * @brief getErrorMessage returns parsing error message if one occurred
         * @return the message occurred
         */
        QString getErrorMessage(void);

        /**
         * @brief isParsingErrorOccurred checks if parsing error occurred
         * @return true if error occurred
         */
        bool isParsingErrorOccurred(void);

        virtual antlrcpp::Any visitFormula(EgcParser::FormulaContext *ctx) override;
        virtual antlrcpp::Any visitEquality(EgcParser::EqualityContext *ctx) override;
        virtual antlrcpp::Any visitDefinition(EgcParser::DefinitionContext *ctx) override;
        virtual antlrcpp::Any visitRedParenthesisL(EgcParser::RedParenthesisLContext *ctx) override;
        virtual antlrcpp::Any visitUMinus(EgcParser::UMinusContext *ctx) override;
        virtual antlrcpp::Any visitVariable(EgcParser::VariableContext *ctx) override;
        virtual antlrcpp::Any visitBracketOp(EgcParser::BracketOpContext *ctx) override;
        virtual antlrcpp::Any visitDifferential(EgcParser::DifferentialContext *ctx) override;
        virtual antlrcpp::Any visitRoot(EgcParser::RootContext *ctx) override;
        virtual antlrcpp::Any visitExponent(EgcParser::ExponentContext *ctx) override;
        virtual antlrcpp::Any visitMulDiv(EgcParser::MulDivContext *ctx) override;
        virtual antlrcpp::Any visitSqrt(EgcParser::SqrtContext *ctx) override;
        virtual antlrcpp::Any visitEmptyBinOp(EgcParser::EmptyBinOpContext *ctx) override;
        virtual antlrcpp::Any visitIteratorL(EgcParser::IteratorLContext *ctx) override;
        virtual antlrcpp::Any visitRedParenthesisR(EgcParser::RedParenthesisRContext *ctx) override;
        virtual antlrcpp::Any visitPlusMinus(EgcParser::PlusMinusContext *ctx) override;
        virtual antlrcpp::Any visitFunction(EgcParser::FunctionContext *ctx) override;
        virtual antlrcpp::Any visitParenthesis(EgcParser::ParenthesisContext *ctx) override;
        virtual antlrcpp::Any visitEmpty(EgcParser::EmptyContext *ctx) override;
        virtual antlrcpp::Any visitIteratorR(EgcParser::IteratorRContext *ctx) override;
        virtual antlrcpp::Any visitNumber(EgcParser::NumberContext *ctx) override;
        virtual antlrcpp::Any visitNatlogarithm(EgcParser::NatlogarithmContext *ctx) override;
        virtual antlrcpp::Any visitLogarithm(EgcParser::LogarithmContext *ctx) override;
        virtual antlrcpp::Any visitIntegral(EgcParser::IntegralContext *ctx) override;
        virtual antlrcpp::Any visitCreateArglist(EgcParser::CreateArglistContext *ctx) override;
        virtual antlrcpp::Any visitAddArgument(EgcParser::AddArgumentContext *ctx) override;

private:
        /**
         * @brief addBinaryExpression add binary Expression to the current AST
         * @param type the type of binary expression to create
         * @param node0 the first node to add to the binary expression
         * @param node1 the second (right) node to add to the binary expression
         * @return a pointer to the binary expression created
         */
        EgcNode* addBinaryExpression(EgcNodeType type, EgcNode* node0,
                                           EgcNode* node1);

        /**
         * @brief addUnaryExpression add unary Expression to the current AST
         * @param type the type of binary expression to create
         * @param node0 the node to add to the unary expression
         * @return a pointer to the unary expression created
         */
        EgcNode* addUnaryExpression(EgcNodeType type, EgcNode* node0);

        /**
         * @brief addFunction add a user defined function to the formula
         * @param fncName the function name of the function to create
         * @param argList the argument list with all expression to add to the function. The List can be integrated
         * directly into the function.
         * @return pointer to the function created
         */
        EgcNode* addFunction(const std::string& fncName, EgcNode* argList);
        /**
         * @brief isBuiltinOperation checks if a builtin operation of the maxima kernel was given which must be handled
         * separately
         * @param fncName name of the function
         * @param node the node to use for the special operation
         * @return pointer to the operation created
         */
        EgcNode* isBuiltinOperation(const std::string& fncName, EgcNode* node);

        /**
         * @brief changeFlexExpressionType change the given argList form a function node type to the type given (move the cildren)
         * @param type the type to change to
         * @param argList the arg list to use
         * @return pointer to the new type
         */
        EgcNode* changeFlexExpressionType(EgcNodeType type, EgcNode *argList);
        /**
         * @brief updateIterator to point to the given node
         * @param node0 the node to update the iterator with
         * @param i iterator data to update
         * @return node0
         */
        EgcNode* updateIterator(EgcNode* node0, int i);
        /**
         * @brief updateIterator if any iterator points to the given original pointer
         * @param original the original pointer to check for (will be replaced by new pointer)
         * @param new_pointer the new pointer that replaces the old one
         * @return pointer that has been replaced
         */
        EgcNode* updateIterator(EgcNode* original, EgcNode* new_pointer);

        /**
         * @brief createArgList creates an argument list that is integrated later on in the function where it is used in
         * @param expression the expression to add to the argument list
         * @return a pointer to the created argument list
         */
        EgcNode* createArgList(EgcNode* expression);

        /**
         * @brief addArgument adds an argument to the argument list given
         * @param expressionToAdd the expression (argument) to add
         * @param argumentList the argument list to use to add the argument to
         * @return a pointer to the changed argument list
         */
        EgcNode* addArgument(EgcNode* expressionToAdd, EgcNode *argumentList);
        /**
         * @brief addEmptyNode add an empty node. USE THIS FOR DEBUG PURPOSES ONLY.
         * @return return a EgcNode of an emtpy node
         */
        EgcNode* addEmptyNode(void);
        
        /**
         * @brief addDanglingNode add a dangling node to the set of dangling nodes. So it is easy to delete them later
         * in case of an error during parsing.
         * @param node the node to add
         */
        void addDanglingNode(EgcNode* node);

        /**
         * * @brief removeDanglingNode remove a dangling node from the set of dangling nodes. So it is easy to delete
         * them later in case of an error during parsing.
         * @param node the node to remove
         */
        void setNotDangling(EgcNode* node);

        /**
         * @brief deleteDanglingNodes delete all dangling nodes in case of an error during build up of the AST
         */
        void deleteDanglingNodes(void);

        /**
         * @brief addDifferentialExpression add a differential node
         * @param argList the arg list to use
         * @return pointer to the new type
         */
        EgcNode* addDifferentialExpression(EgcNode *argList);

        /**
         * @brief addDivisionExpression add a divison Expression to the current AST
         * @param node0 the first node to add to the division expression (dividend)
         * @param node1 the second (right) node to add to the division expression (divisor)
         * @return a pointer to the binary expression created
         */
        EgcNode* addDivisionExpression(EgcNode* node0, EgcNode* node1);

        /**
         * @brief removeParenthesisChild cut out the child from the tree if the child is a parenthesis child. The
         * grandchild becomes the child of the parent.
         * @param parenthesisNode the child to cut out.
         * @return true if everything worked well, false otherwise.
         */
        bool removeParenthesisChild(EgcNode& parenthesisNode);
        /**
         * @brief addSqrtExpression add a square root to the tree. This is some sort of a hack, since a root node is a
         * binary node, but this is constructed with just one argument (unary), the second node (2) is constructed in
         * the background
         * @param node0 the radicand to use
         * @return
         */
        EgcNode* addSqrtExpression(EgcNode* node0);

        //Q_DISABLE_COPY(FormulaInterpreter)

        /**
         * @brief setRootNode stores the root node inside the FormulaInterpreter class.
         * @param node root node of the parsed formula to remember.
         */
        void setRootNode(EgcNode* node);

        QScopedPointer<EgcNode> m_rootNode;             ///< the base node of the formula
        unsigned int m_location;                        ///< Used by scanner
        QSet<EgcNode*> m_danglingNodes;                 ///< holds the dangling nodes during AST is built up
        EgcNode* m_iterPointer1;                        ///< special pointer that is given by some visitors
        EgcNode* m_iterPointer2;                        ///< special pointer that is given by some visitors
        EgcNode* m_iterPointer3;                        ///< special pointer that is given by some visitors
        bool m_parseKernelResult;                       ///< post process Formula  e.g. removing unneccessary parenthesis
        QString m_errorMessage;                         ///< error message when error during parsing.
        bool m_isErrorOccurred;
};



#endif // FORMULAINTERPRETER_H
