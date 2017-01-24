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
#ifndef GRID_H
#define GRID_H

#include <QSizeF>
#include <QPointF>

class EgCasScene;

namespace egcas {

/**
 * @brief The Grid class bundles all functionality regarding the egcas worksheet grid
 */
class Grid
{
public:
        Grid(EgCasScene& scene);
        Grid(EgCasScene& scene, QSizeF size);
        /**
         * @brief setGrid set the grid size
         * @param size the width and height of the grid
         */
        void setGrid(QSizeF size);
        /**
         * @brief grid the current grid size
         * @return the grid size
         */
        QSizeF grid(void) const;
        /**
         * @brief snap snaps the given position to the grid
         * @param point the point to snap to the grid
         * @return the snapped position
         */
        QPointF snap(const QPointF& point) const;
        /**
         * @brief activate activates the grid
         * @param activate if true the grid will be activated, if false deactivated
         */
        void activate(bool activate);
        /**
         * @brief isActivated check if grid is activated
         * @return true if activated, false otherwise
         */
        bool isActivated(void) const;


private:
        QSizeF m_grid;                  ///< width and height of the grid
        EgCasScene& m_scene;            ///< reference to scene
        bool m_activated;               ///< if grid is activated or not
};

}

#endif // GRID_H
