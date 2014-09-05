#ifndef TST_EGCASTEST_STRUCTURAL_H
#define TST_EGCASTEST_STRUCTURAL_H

#include "../../src/structural/egcunaryexpressionnode.h"
#include "../../src/structural/egcbinaryexpressionnode.h"


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
