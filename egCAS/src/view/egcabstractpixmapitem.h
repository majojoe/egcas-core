/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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

#ifndef EGCABSTRACTPIXMAPITEM_H
#define EGCABSTRACTPIXMAPITEM_H

#include <QtGlobal>
#include <QPixmap>
#include "egcasiteminterface.h"

class EgcAbstractPixmapItem : public EgcasItemInterface
{
public:
        /**
         * @brief ~EgcAbstractFormulaItem virtual destructor in order to be able to delete subclasses
         */
        virtual ~EgcAbstractPixmapItem() {}
        /**
         * @brief setScaleFactor set the scale factor of the pixmap
         * @param scaleFactor the scale factor to set
         */
        virtual void setScaleFactor(qreal scaleFactor) = 0;
        /**
         * @brief getSize returns the size of the pixmap
         * @return the size of the pixmap as QSize
         */
        virtual QSize getSize(void) = 0;
        /**
         * @brief setPixmap sets the pixmap to use for the item
         * @param pixmap the pixmap to set
         */
        virtual void setPixmap(QPixmap pixmap) = 0;
        /**
         * @brief getPixmap returns the pixmap of the current item
         * @return the pixmap of the current item
         */
        virtual QPixmap getPixmap(void) = 0;
};

#endif // EGCABSTRACTPIXMAPITEM_H
