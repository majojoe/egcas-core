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
#ifndef EGCWORKSHEET_H
#define EGCWORKSHEET_H

#include <QSizeF>
#include <QPointF>
#include <QRectF>

class EgCasScene;

/**
 * @brief The EgcWorksheet class holds all information about size and margins of the worksheet
 */
class EgcWorksheet
{
public:
        EgcWorksheet(EgCasScene& scene);

        /**
         * @brief getSize returns size of one worksheet
         * @return the size of the worksheet
         */
        QSizeF getSize(void);
        /**
         * @brief setSize sets the size of a worksheet
         * @param size the size to set
         */
        void setSize(QSizeF size);
        /**
         * @brief getLeftMargin returns the left margin of the worksheet
         * @return the left margin
         */
        qreal getLeftMargin(void) const;
        /**
         * @brief setLeftMargin sets the left margin of the worksheet
         * @param margin the margin to set
         */
        void setLeftMargin(qreal margin);
        /**
         * @brief getRightMargin returns the right margin of the worksheet
         * @return the right margin
         */
        qreal getRightMargin(void) const;
        /**
         * @brief setRightMargin sets the right margin of the worksheet
         * @param margin the margin to set
         */
        void setRightMargin(qreal margin);
        /**
         * @brief getTopMargin returns the top margin of the worksheet
         * @return the top margin
         */
        qreal getTopMargin(void) const;
        /**
         * @brief setTopMargin sets the top margin of the worksheet
         * @param margin the margin to set
         */
        void setTopMargin(qreal margin);
        /**
         * @brief getBottomMargin returns the bottom margin of the worksheet
         * @return the bottom margin
         */
        qreal getBottomMargin(void) const;
        /**
         * @brief setBottomMargin sets the bottom margin of the worksheet
         * @param margin the margin to set
         */
        void setBottomMargin(qreal margin);
        /**
         * @brief snapWorksheet snap the given point into the worksheet
         * @param point can be any point of the scene
         * @return the corrected point that is inside the worksheet
         */
        QPointF snapWorksheet(const QPointF& point) const;
        /**
         * @brief snapWorksheet snap the given item rectangle into the worksheet
         * @param item is the rectangular shape of the item on the scene
         * @return the corrected position of the item inside the worksheet
         */
        QPointF snapWorksheet(const QRectF& item) const;
        /**
         * @brief itemWrapsToNewPage check if given item wraps to new page
         * @param item the rectangle that is used by the item on the scene
         * @param yOffset the offset in y direction by that the item shall be moved
         * @return true if the item is on a new page after the move
         */
        bool itemWrapsToNewPage(const QRectF& item, qreal yOffset);
        /**
         * @brief itemOverlapsPageWidth checks if the item overlaps with its width the worksheet width. Furthermore
         * it calculates the offset by which the item overlaps the worksheet page
         * @param item the rectangular shape of the item on the scene
         * @param overlapsPageBy 0 if the item does not overlap the worksheet or the offset by which the item overlaps
         * the worksheet in x direction, if so. Scene can be extended by the offset then to view the whole item.
         * @return true if the item overlaps the x direction of the worksheet, false otherwise.
         */
        bool itemOverlapsPageWidth(const QRectF& item, qreal& overlapsPageBy);
        /**
         * @brief isVisible checks if given point is within a visible area of a worksheet
         * @param point the point to check
         * @return true if the point is within a visible area, false otherwise
         */
        bool isVisible(QPointF point) const;
        /**
         * @brief pageAtPoint returns the page index for the given point
         * @param point the point to calculate the page index for
         * @return the page index
         */
        quint32 pageAtPoint(QPointF point) const;
        /**
         * @brief activeArea returns the active/visible area for the given page index
         * @param pageIndex the page index we want to get the visible area for
         * @return the active area
         */
        QRectF activeArea(quint32 pageIndex) const;

private:
        QSizeF m_size;                  ///< the worksheet size
        qreal m_leftMargin;             ///< the left margin of the worksheet
        qreal m_rightMargin;            ///< the right margin of the worksheet
        qreal m_topMargin;              ///< the top margin of the worksheet
        qreal m_bottomMargin;           ///< the bottom margin of the worksheet
        EgCasScene& m_scene;            ///< reference to scene

};

#endif // EGCWORKSHEET_H
