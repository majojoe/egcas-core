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

#ifndef EGCASITEMINTERFACE_H
#define EGCASITEMINTERFACE_H

class QPointF;

/**
 * @brief The EgcasItemInterface class implements an interface for all classes that are derived from QGraphicsItem
 */
class EgcasItemInterface
{
public:
        ///std contructor
        EgcasItemInterface() {}
        ///std destructor
        virtual ~EgcasItemInterface();
        /**
         * @brief getPosition needs to be overwritten by subclasses to get the position of the item
         * @return the Position of the item
         */
        virtual QPointF getPosition( void ) const = 0;
        /**
         * @brief setPosition needs to be overwritten by subclasses to set the position of the item
         * @param point the position to set.
         */
        virtual void setPos(const QPointF& point) = 0;
        /**
         * @brief aboutToBeDeleted checks if item is about to be deleted, e.g. if a formula is empty and one hits del
         * key this function will return true, and false otherwise
         * @return true if pressing del or backspace shall delete the item
         */
        virtual bool aboutToBeDeleted(void) { return true;}
        /**
         * @brief operator< operator overload of this class to be able to sort the items according to their position
         * @param rhs the right hand side operator
         * @return true if this object is smaller than  the given object
         */
        bool operator< ( const EgcasItemInterface &rhs);
};

#endif // EGCASITEMINTERFACE_H
