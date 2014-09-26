#ifndef TST_EGCASTEST_STRUCTURAL_H
#define TST_EGCASTEST_STRUCTURAL_H

#include "structural/egcunaryexpressionnode.h"
#include "structural/egcbinaryexpressionnode.h"
#include "structural/egcrootexpressionnode.h"
#include "structural/egcnumberexpressionnode.h"
#include "structural/iterator/egcexpressionnodeiterator.h"
#include "structural/egcformulaexpression.h"

class EgcUnaryExpressionNodeTestChild : public EgcUnaryExpressionNode
{
public:
        ///std constructor
        EgcUnaryExpressionNodeTestChild() {deleted = false;}
        /// destructor
        virtual ~EgcUnaryExpressionNodeTestChild() {deleted = true;}
        static bool deleted;
};

class EgcUnaryExpressionNodeTest : public EgcUnaryExpressionNode
{
};

class EgcBinaryExpressionNodeTestChild : public EgcBinaryExpressionNode
{
public:
        ///std constructor
        EgcBinaryExpressionNodeTestChild() {deleted = false;}
        /// destructor
        virtual ~EgcBinaryExpressionNodeTestChild() {deleted = true;}
        static bool deleted;
};

class EgcBinaryExpressionNodeTestChild2 : public EgcBinaryExpressionNode
{
public:
        ///std constructor
        EgcBinaryExpressionNodeTestChild2() {deleted = false;}
        /// destructor
        virtual ~EgcBinaryExpressionNodeTestChild2() {deleted = true;}
        static bool deleted;
};

class EgcBinaryExpressionNodeTest : public EgcBinaryExpressionNode
{
};

#endif // TST_EGCASTEST_STRUCTURAL_H
