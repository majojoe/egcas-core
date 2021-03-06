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

#include "formulascrelement.h"
#include <QString>

const char emptyElement[] = "_empty";
const char rightCursorPointer[] = "_>R";
const char leftCursorPointer[] = "_<L";

TempDataScrIter::TempDataScrIter() : m_id{0}, m_node{nullptr}, m_subpos{0}, m_left_side{true}
{

}

FormulaScrElement::FormulaScrElement() : FormulaScrElement{QString::null}
{
}

FormulaScrElement::FormulaScrElement(QString value) : m_value{value}, m_cAdh{CursorAdhesion::low}, m_node{nullptr},
                                         m_subpos_min{0}, m_subpos_max{0},
                                         m_isSegmented{false}, m_positionMarker{PositionMarker::noMarker}
{
}

bool FormulaScrElement::hasPositionMarker() const
{
        if (m_positionMarker != PositionMarker::noMarker)
                return true;

        return false;
}

void FormulaScrElement::markPosition(bool rightSide)
{
        if (rightSide)
                m_positionMarker = PositionMarker::rightMarker;
        else
                m_positionMarker = PositionMarker::leftMarker;
}

void FormulaScrElement::resetPositionMarker()
{
        m_positionMarker = PositionMarker::noMarker;
}

bool FormulaScrElement::isRightPositionMarker()
{
        if (m_positionMarker == PositionMarker::rightMarker)
                return true;

        return false;
}

bool FormulaScrElement::isEmptyElement() const
{
        if (m_value == QString(emptyElement))
                return true;

        return false;
}

bool FormulaScrElement::isRightCursorPointer() const
{
        if (m_value == QString(rightCursorPointer))
                return true;

        return false;
}

bool FormulaScrElement::isLeftCursorPointer() const
{
        if (m_value == QString(leftCursorPointer))
                return true;

        return false;
}
