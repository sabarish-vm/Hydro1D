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
#ifndef BONDI_HPP
#define BONDI_HPP

#include "Cell.hpp"           // Cell classe
#include "SafeParameters.hpp" // Safe way to include Parameters.hpp
#include "DerivedParameters.hpp"

#include <cmath>
#include <iostream>

/*! @brief Bondi density: density at the neutral Bondi radius (in internal units
 *  of M L^-3). */
#define BONDI_DENSITY (BONDI_DENSITY_IN_SI / UNIT_DENSITY_IN_SI)

/*! @brief Neutral Bondi radius (in internal units of L). */
#define RBONDI (0.5 * G_INTERNAL * MASS_POINT_MASS / pow(SOUND_INFINITY,2))

// equation of state functionality for EOS_BONDI
#if EOS == EOS_BONDI

namespace BondiFunc {
    inline double cs(const double& rho) {
            double a2 = POLYTROPIC_CONSTANT * GAMMA * pow(rho,GAMMA-1) ;
            double a = std::sqrt(a2);
            return a;
        }
}


#define initial_pressure(cell) cell._P = POLYTROPIC_CONSTANT * pow(cell._rho,GAMMA)

/**
 * @brief Conversion function called during the primitive variable conversion
 * for the given cell.
 *
 * This is the special equation of state that increase the pressure for ionised
 * gas.
 *
 * @param cell Cell.
 */
#define update_cs(cell)                                                  \
  cell._cs = BondiFunc::cs(cell._rho);

#define update_pressure(cell)                                                  \
  cell._P = POLYTROPIC_CONSTANT * pow(cells[i]._rho,GAMMA);\

#endif // EOS == EOS_BONDI

// boundary condition functionality
#if BOUNDARIES == BOUNDARIES_BONDI

/**
 * @brief Initialize variables used for the boundary conditions.
 *
 * We need to initialize the outer boundary variables.
 */
#define boundary_conditions_initialize()                                       \
std::cout<<"BC_initialize"<<std::endl;
/**
 * @brief Apply boundary conditions after the primitive variable conversion.
 */
#define boundary_conditions_primitive_variables()                              \
  /* impose the Bondi solution at the boundaries */                            \
  /* lower boundary: outflow */                                                \
  cells[0]._rho = cells[1]._rho;                                               \
  cells[0]._u = cells[1]._u;                                                   \
  cells[0]._P = cells[1]._P;                                                   \
                                                                               \
  /* upper boundary: neutral Bondi solution */                                 \
  cells[ncell + 1]._rho = RHO_INFINITY;                                        \
  cells[ncell + 1]._u = VELOCITY_INFINITY;                                     \
  cells[ncell + 1]._P = PRESSURE_INFINITY;

/**
 * @brief Apply boundary conditions after the gradient computation.
 */
#define boundary_conditions_gradients()                                        \
  /* lower boundary: outflow */                                                \
  {                                                                            \
    cells[0]._grad_rho = cells[1]._grad_rho;                                   \
    cells[0]._grad_u = cells[1]._grad_u;                                       \
    cells[0]._grad_P = cells[1]._grad_P;                                       \
  }                                                                            \
                                                                               \
  /* upper boundary: compute gradient using the known expression outside rmax  \
   */                                                                          \
  {                                                                            \
    cells[ncell + 1]._grad_rho = cells[ncell]._grad_rho;                       \
   cells[ncell + 1]._grad_u = cells[ncell]._grad_u;                            \
    cells[ncell + 1]._grad_P = cells[ncell]._grad_P;                           \
  }

#endif // BOUNDARIES == BOUNDARIES_BONDI

// Bondi initial condition functionality
#if IC == IC_BONDI
/**
 * @brief Initialize the given cells.
 *
 * We start with a constant density and velocity everywhere, the value of which
 * corresponds to the inflow values.
 *
 * @param cells Cells to initialize.
 * @param ncell Number of cells.
 */
#define initialize(cells, ncell)                                               \
  _Pragma("omp parallel for") for (unsigned int i = 1; i < ncell + 1; ++i) {   \
    cells[i]._rho = RHO_INFINITY;                                              \
    cells[i]._u = VELOCITY_INFINITY;                                           \
    cells[i]._P = POLYTROPIC_CONSTANT * pow(cells[i]._rho,GAMMA);              \
    const double r2 = cells[i]._midpoint * cells[i]._midpoint;                 \
    cells[i]._a = -G_INTERNAL * MASS_POINT_MASS / r2;                          \
    cells[i]._cs = std::sqrt(POLYTROPIC_CONSTANT_IN_SI * GAMMA *               \
          pow(cells[i]._rho *UNIT_DENSITY_IN_SI,GAMMA-1));                     \
  }

#endif // IC == IC_BONDI

#endif // BONDI_HPP
