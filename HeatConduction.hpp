/*******************************************************************************
 * This file is part of HydroCodeSpherical1D
 * Copyright (C) 2024 Heat Conduction Extension
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
 * @file HeatConduction.hpp
 *
 * @brief Heat conduction physics for 1D spherical hydrodynamics.
 *
 * @author Heat Conduction Extension
 */
#ifndef HEAT_CONDUCTION_HPP
#define HEAT_CONDUCTION_HPP

#include "SafeParameters.hpp"
#include "PhysicalConstants.hpp"

#if HEAT_CONDUCTION == HEAT_CONDUCTION_ON

#define compute_heat_conduction_timestep(cells, ncell) \
  _Pragma("omp parallel for reduction(min:min_heat_dt)") \
  for (uint_fast32_t i = 1; i < ncell + 1; ++i) { \
    const double T = cells[i]._P / (cells[i]._rho * BOLTZMANN_K_IN_SI); \
    /* Thermal diffusivity: α = κ / (ρ * c_v) */ \
    /* For ideal gas: c_v = 1.5 * k_B / m (assuming monatomic) */ \
    const double thermal_diffusivity = THERMAL_CONDUCTIVITY / \
                                      (cells[i]._rho * 1.5 * BOLTZMANN_K_IN_SI); \
    \
    /* Stability constraint: dt < factor * dr² / α */ \
    const double heat_dt = HEAT_CONDUCTION_TIMESTEP_FACTOR * \
                          cells[i]._V * cells[i]._V / thermal_diffusivity; \
    \
    min_heat_dt = std::min(1e10, heat_dt); \
  } \
  min_heat_dt

#else // HEAT_CONDUCTION == HEAT_CONDUCTION_OFF

/* Heat conduction disabled - do nothing */
#define compute_heat_conduction_fluxes(cells, ncell)
#define apply_heat_conduction_source_terms(cells, ncell, dt)
#define compute_heat_conduction_timestep(cells, ncell) 1e10
#define initialize_heat_conduction(cells, ncell)

#endif // HEAT_CONDUCTION == HEAT_CONDUCTION_ON

#endif // HEAT_CONDUCTION_HPP
