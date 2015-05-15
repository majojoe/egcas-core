#ifndef TST_EGCASTEST_STRUCTURAL_H
#define TST_EGCASTEST_STRUCTURAL_H

#include "structural/egcnodes.h"
#include "structural/iterator/egcexpressionnodeiterator.h"
#include "structural/egcformulaexpression.h"
#include "structural/egcnodecreator.h"
#include "structural/visitor/egcnodevisitor.h"
#include "structural/visitor/egcmaximavisitor.h"
#include "structural/visitor/egcmathmlvisitor.h"

class EgcUnaryNodeTestChild : public EgcUnaryNode
{
public:
        ///std constructor
        EgcUnaryNodeTestChild() {deleted = false;}
        /// destructor
        virtual ~EgcUnaryNodeTestChild() {deleted = true;}
        static bool deleted;
};

class EgcUnaryNodeTest : public EgcUnaryNode
{
};

class EgcBinaryNodeTestChild : public EgcBinaryNode
{
public:
        ///std constructor
        EgcBinaryNodeTestChild() {deleted = false;}
        /// destructor
        virtual ~EgcBinaryNodeTestChild() {deleted = true;}
        static bool deleted;
};

class EgcBinaryNodeTestChild2 : public EgcBinaryNode
{
public:
        ///std constructor
        EgcBinaryNodeTestChild2() {deleted = false;}
        /// destructor
        virtual ~EgcBinaryNodeTestChild2() {deleted = true;}
        static bool deleted;
};

class EgcBinaryNodeTest : public EgcBinaryNode
{
};

#endif // TST_EGCASTEST_STRUCTURAL_H
