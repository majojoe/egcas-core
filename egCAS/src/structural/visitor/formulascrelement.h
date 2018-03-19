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
#ifndef FORMULASCRELEMENT_H
#define FORMULASCRELEMENT_H

#include <QString>
#include <QVector>
#include <QLineF>

class EgcNode;

/**
 * @brief The CursorAdhesion enum reflects the cursor adhesion of a symbol. If the cursor is between e.g. a normal and a
 * strong node, the cursor will be shown with the dimensions of the node with the strong adhesion
 */
enum class CursorAdhesion {
        invisible,      ///< if the cursor at the current element position is invisible
        low,            ///< low cursor adhesion
        normal,         ///< standard (for most other symbols)
        strong,         ///< strong is e.g. for alpha numeric symbols
        ultra,          ///< to override strong adhesion of alpha numeric symbols
};

enum class PositionMarker {
        noMarker,               ///< the element has no position marked at all
        leftMarker,             ///< the position is marked at the left side of the element
        rightMarker             ///< the position is marked at the right side of the element
};

class TempDataScrIter
{
public:
        TempDataScrIter();
        quint32 m_id;           ///< id that describes formula elements that belong to each other
        EgcNode* m_node;        ///< pointer to the node that created this element
        quint32 m_subpos;       ///< saves the subposition of the element, normally 0
        bool m_left_side;       ///< if true, cursor data shall be taken from left edge of boundary rect, if false from right one
};


class FormulaScrElement
{
public:      
        enum SideNode{nodeLeftSide, nodeRightSide, nodeMiddle}; ///< at which section of the associated nodes the visible element belongs to
        FormulaScrElement();
        /**
         * @brief hasPositionMarker checks if the element has a marked position
         * @return true if the position is marked, false if not
         */
        bool hasPositionMarker(void) const;
        /**
         * @brief markPosition mark the current element as position to remember
         * @param rightSide if true the right side of the element is marked as position to remember, if false, the left
         * side will be marked as position to remember.
         */
        void markPosition(bool rightSide = true);
        /**
         * @brief resetPositionMarker reset the current element to an unmarked element position
         */
        void resetPositionMarker(void);
        /**
         * @brief isRightPositionMarker check if it is a right position marker or a left one
         * @return true if the current element has a position marker on the right side, false otherwise
         */
        bool isRightPositionMarker(void);
        QString m_value;        ///< formula element that is visible as entity on the screen
        CursorAdhesion m_cAdh;  ///< cursor adhesion of the node
        EgcNode* m_node;        ///< pointer to node this element is directly associated with
        quint32 m_subpos_min;   ///< subposition of this element (minimum subposition if more elements)
        quint32 m_subpos_max;   ///< maximum subposition of this element
        enum SideNode m_sideNode; ///< which section of the node this visible element is associated to
        bool m_isSegmented;     ///< true if this element is segmented over more elements, meaning if this element is deleted, the others (pointing to the same m_node) will also be deleted
        PositionMarker m_positionMarker;///< marker for position
        TempDataScrIter lTemp;  ///< temporary data for left side of cursor element
        TempDataScrIter rTemp;  ///< temporary data for right side of cursor element
};

/**
 * @brief FormulaScrVector defines a vector of type FormulaScrElement
 */
typedef QVector<FormulaScrElement> FormulaScrVector;

#endif // FORMULASCRELEMENT_H
