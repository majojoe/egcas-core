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

#include "egcentitylist.h"
#include "document/egcdocument.h"

EgcEntityList::EgcEntityList(EgcAbstractEntityList* parent) : m_index(0), m_parent(parent)
{
}

EgcEntityList::~EgcEntityList()
{
        EgcEntity* i;
        foreach (i, m_list) {
                delete i;
        }
}

void EgcEntityList::sort(void)
{
        qSort(m_list);
}

void EgcEntityList::addEntity(EgcEntity* entity)
{
        m_list.append(entity);
        sort();
}

bool EgcEntityList::deleteEntity(EgcEntity* entity)
{
        int i = m_list.indexOf(entity);
        if (i > 0) {
                delete m_list.takeAt(i);
        }
}

EgcEntity* EgcEntityList::takeEntity(EgcEntity* entity)
{
        EgcEntity* retval = nullptr;

        int i = m_list.indexOf(entity);
        if (i > 0) {
                retval = m_list.takeAt(i);
        }

        return retval;
}

void EgcEntityList::toStart(void)
{
        m_index = 0;
}

EgcEntity* EgcEntityList::next(void)
{
        EgcEntity* retval = nullptr;

        if (m_index >= 0 && m_index < m_list.count()) {
                retval = m_list.at(m_index);
                m_index++;
                if (m_index >= m_list.count())
                        m_index = -1;
        }

        return retval;
}

EgcDocument* EgcEntityList::getDocument(void)
{
        if (m_parent)
                return m_parent->getDocument();
        else
                return nullptr;
}

EgcAbstractEntityList* EgcEntityList::getParent(void)
{
        return m_parent;
}

EgcEntity* EgcEntityList::createEntity(EgcEntityType type, QPointF point)
{
        EgcDocument* doc = getDocument();
        if (doc) {
                return doc->createEntity(type, this, point);
        } else {
                return nullptr;
        }
}
