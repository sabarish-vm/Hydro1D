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
 * @file Potential.hpp
 *
 * @brief External potential.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#ifndef POTENTIAL_HPP_
#define POTENTIAL_HPP_

#include <omp.h>

#include <memory>

#include "./Cell.hpp"
#include "./DerivedParameters.hpp"
#include "./OptionNames.hpp"
/**
 * @brief Add the gravitational acceleration.
 */
template <Potential PT>
void do_gravity(std::unique_ptr<Cell[]>& cells, const uint32_t& ncell) {
  if constexpr (PT == Potential::POINT_MASS) {
#pragma omp parallel for
    for (uint_fast32_t i = 1; i < ncell + 1; ++i) {
      const double r = cells[i]._midpoint;
      const double a = -G_INTERNAL * MASS_POINT_MASS / (r * r);
      const double m = cells[i]._V * cells[i]._rho;
      cells[i]._p += 0.5 * cells[i]._dt * a * m;
      cells[i]._E += 0.5 * cells[i]._dt * a * m * cells[i]._u;
    }
  } else if constexpr (PT == Potential::NONE) {
  }
}

/**
 * @brief Do the gravitational half time step prediction for the given cell.
 *
 * @param cell Cell.
 * @param half_dt Half the particle time step (in internal units of T).
 */
template <Potential PT>
void add_gravitational_prediction(Cell cell, const double& half_dt) {
  if constexpr (PT == Potential::NONE) {
    cell._u += half_dt * cell._a;
  } else {
  }
}

#endif  // POTENTIAL_HPP_
