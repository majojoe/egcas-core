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

#ifndef EGCALNUMNODE_H
#define EGCALNUMNODE_H

#include <QtGlobal>
#include <QString>
#include <QRegularExpression>
#include "../specialNodes/egcnode.h"

class EgcSubindNodeIter;

/**
 * @brief The EgcAlnumNode class is a class that holds leafes with alphanumerice identifier
 *
 * It is also possible to store mathml symbol codes e.g. &Alpha; inside the varible node. Following codes for ampersand
 * '&' and ";" are used:
 * _2  : as code for the ampersand '&'
 * _3  : as code for ";"
 * These symbols cannot be represented by a cas kernel or have different meaning, therefore the are stuffed.
 */
class EgcAlnumNode : public EgcNode
{
        friend class EgcSubindNodeIter;

        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcAlnumNode, EgcNodeType::AlnumNode);

public:
        ///std constructor
        EgcAlnumNode();
        /**
         * @brief EgcAlnumNode alternative constructor
         * @param firstCharMightBeNumber if true the insert method will allow that the first char can be a number
         */
        EgcAlnumNode(bool firstCharMightBeNumber);
        ///destructor
        virtual ~EgcAlnumNode();
        /**
         * @brief setValue set the value of this leaf
         * @param value the number value to be set
         */
        virtual void setValue(const QString& value);
        /**
         * @brief getValue returns the value saved in this class
         * @return the value of this object
         */
        virtual QString getValue(void) const;
        /**
         * @brief operator== comparison operator overload
         * @param node the node to compare against
         * @return true if the trees are equal
         */
        virtual bool operator==(const EgcNode& node) const override;
        /**
         * @brief nrSubindexes returns the number of subindexes of this node. This can be e.g. the number of characters
         * of a number or variable
         * @return 0 if not overwritten, since a std. node has no subindexes
         */
        virtual int nrSubindexes(void) const override;
        /**
         * @brief setValue set the stuffed variable name (value)
         * @param varName the variable name as a string. This can include the stuffed special signs (a "_" in the
         * variable name is stuffed into "__", and special signs are encoded with html escapes where 'ampersand'&'
         * and ';' are again encoded with '_2' and '_3'. As a result all characters will be ASCII conform then and can
         * be used by the calc kernel).
         */
        virtual void setStuffedValue(const QString& varName);
        /**
         * @brief getStuffedValue returns the stuffed name (with stuffed special signs)
         * @return the stuffed variable name (a "_" in the variable name is stuffed into "__",
         * and special signs are encoded with html escapes where 'ampersand'&' and ';' are again encoded with '_2' and
         * '_3'. As a result all characters will be ASCII conform then and can be used by the calc kernel).
         */
        virtual QString getStuffedValue(void);
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * A variable expression is valid if the value is not empty.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief visibleSigns find out where the node has visible signs (e.g. a division node has visible signs in the
         * middle of the container)
         * @param side the side to test for visible signs
         * @return true if the given side of the node has visible signs.
         */
        virtual bool visibleSigns(EgcNodeSide side) const override;
        /**
         * @brief isAtomicChild if a node is a child of a atomic node it may not be deleted seperately. There may be a
         * parent that is marked as atomic (is deleteable), then the parent must be deleted in order to delete the not
         * deleteable sub-node. This functionality is in some ways comparable to a composition. The also must not be
         * inserted a node in between (this would change the structure).
         * @return true if node is a child of a atomic node, false if NOT a child of a atomic node (majority).
         */
        virtual bool isAtomicChild(void) const override;

protected:
        /**
         * @brief insert insert a character at the given position
         * @param character the character to insert
         * @param position the position where to insert the given character
         * @return true if the operation was successful, false otherwise
         */
        virtual bool insert(QChar character, int position);
        /**
         * @brief remove a charcter at the given position
         * @param position the position at which to remove a character
         * @return true if the operation was successful, false otherwise
         */
        virtual bool remove(int position);

private:
        QString m_value;                                ///< the value of the number
        bool m_firstCharMightBeNumber;                  ///< if true, first char can also be a number
        static QRegularExpression s_validator;          ///< a validator for character inputs
        static QRegularExpression s_ampersand;          ///< regex to replace ampersand
        static QRegularExpression s_ampersandBegin;     ///< regex to replace ampersand at the beginning
        static QRegularExpression s_semi;               ///< regex to replace semicolon
        static QRegularExpression s_semiBegin;          ///< regex to replace semicolon at the beginning
        static bool s_regexInitialized;                 ///< is the regex already initialized
};

#endif // EGCALNUMNODE_H
