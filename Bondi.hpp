/*******************************************************************************
 * This file is part of HydroCodeSpherical1D
 * Copyright (C) 2017 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 *
 * HydroCodeSpherical1D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HydroCodeSpherical1D is distributed in the hope that it will be useful,
 * but WITOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with HydroCodeSpherical1D. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file Bondi.hpp
 *
 * @brief All Bondi problem specific code.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#pragma once
#include <cmath>
#include <memory>

#include "./Cell.hpp"  // Cell classe
#include "./DerivedParameters.hpp"

constexpr double RBONDI = (G_INTERNAL * MASS_POINT_MASS / (SOUND_INFINITY_2));

namespace BondiFunc {
inline double cs(const double &rho) {
  double a2 = POLYTROPIC_CONSTANT * GAMMA * pow(rho, GAMMA - 1);
  double a = std::sqrt(a2);
  return a;
}

void inline initial_pressure(Cell &cell) {
  cell._P = POLYTROPIC_CONSTANT * pow(cell._rho, GAMMA);
}

void inline update_cs(Cell &cell) { cell._cs = BondiFunc::cs(cell._rho); }

void inline update_pressure(Cell &cell) {
  cell._P = POLYTROPIC_CONSTANT * pow(cell._rho, GAMMA);
}

void inline boundary_conditions_initialize(std::unique_ptr<Cell[]> &cells,
                                           u_int32_t ncell) {
  /* impose the Bondi solution at the boundaries */
  /* lower boundary: outflow */
  cells[0]._rho = cells[1]._rho;
  cells[0]._u = cells[1]._u;
  cells[0]._P = cells[1]._P;
  /* upper boundary: neutral Bondi solution */
  cells[ncell + 1]._rho = RHO_INFINITY;
  cells[ncell + 1]._u = VELOCITY_INFINITY;
  cells[ncell + 1]._P = PRESSURE_INFINITY;
}

void inline boundary_conditions_gradients(std::unique_ptr<Cell[]> &cells,
                                          u_int32_t ncell) {
  cells[0]._grad_rho = cells[1]._grad_rho;
  cells[0]._grad_u = cells[1]._grad_u;
  cells[0]._grad_P = cells[1]._grad_P;
  cells[ncell + 1]._grad_rho = cells[ncell]._grad_rho;
  cells[ncell + 1]._grad_u = cells[ncell]._grad_u;
  cells[ncell + 1]._grad_P = cells[ncell]._grad_P;
}

void inline initialize(std::unique_ptr<Cell[]> &cells, u_int32_t ncell) {
  _Pragma("omp parallel for") for (unsigned int i = 1; i < ncell + 1; ++i) {
    cells[i]._rho = RHO_INFINITY;
    cells[i]._u = VELOCITY_INFINITY;
    cells[i]._P = POLYTROPIC_CONSTANT * pow(cells[i]._rho, GAMMA);
    const double r2 = cells[i]._midpoint * cells[i]._midpoint;
    cells[i]._a = -G_INTERNAL * MASS_POINT_MASS / r2;
    cells[i]._cs =
        std::sqrt(POLYTROPIC_CONSTANT_IN_SI * GAMMA *
                  pow(cells[i]._rho * UNIT_DENSITY_IN_SI, GAMMA - 1));
  }
}
}  // namespace BondiFunc
