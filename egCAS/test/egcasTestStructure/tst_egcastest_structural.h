/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
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

#ifndef TST_EGCASTEST_STRUCTURAL_H
#define TST_EGCASTEST_STRUCTURAL_H

#include "egcnodes.h"
#include "iterator/egcnodeiterator.h"
#include "entities/egcformulaentity.h"
#include "egcnodecreator.h"
#include "visitor/egcnodevisitor.h"
#include "visitor/egcmaximavisitor.h"
#include "visitor/egcmathmlvisitor.h"

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
