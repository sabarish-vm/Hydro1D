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

/**
 * @brief Compute heat conduction fluxes between cells.
 * 
 * This computes the heat flux at cell interfaces using Fourier's law:
 * q = -κ ∇T, where κ is the thermal conductivity.
 * 
 * @param cells Cell array
 * @param ncell Number of cells
 */
#define compute_heat_conduction_fluxes(cells, ncell) \
  _Pragma("omp parallel for") \
  for (uint_fast32_t i = 1; i < ncell + 1; ++i) { \
    /* Compute temperatures from pressure and density */ \
    const double T_left = cells[i-1]._P / (cells[i-1]._rho * BOLTZMANN_K_IN_SI); \
    const double T_right = cells[i+1]._P / (cells[i+1]._rho * BOLTZMANN_K_IN_SI); \
    const double T_center = cells[i]._P / (cells[i]._rho * BOLTZMANN_K_IN_SI); \
    \
    /* Compute temperature gradients */ \
    const double dr_left = cells[i]._midpoint - cells[i-1]._midpoint; \
    const double dr_right = cells[i+1]._midpoint - cells[i]._midpoint; \
    \
    const double dT_dr_left = (T_center - T_left) / dr_left; \
    const double dT_dr_right = (T_right - T_center) / dr_right; \
    \
    /* Heat flux: q = -κ ∇T (spherical geometry: area = 4πr²) */ \
    const double area_left = 4. * M_PI * cells[i-1]._midpoint * cells[i-1]._midpoint; \
    const double area_right = 4. * M_PI * cells[i+1]._midpoint * cells[i+1]._midpoint; \
    \
    const double heat_flux_left = -THERMAL_CONDUCTIVITY * dT_dr_left * area_left; \
    const double heat_flux_right = -THERMAL_CONDUCTIVITY * dT_dr_right * area_right; \
    \
    /* Store heat fluxes for later use */ \
    cells[i]._heat_flux_left = heat_flux_left; \
    cells[i]._heat_flux_right = heat_flux_right; \
  }

/**
 * @brief Apply heat conduction source terms to energy equation.
 * 
 * This updates the total energy due to heat conduction:
 * dE/dt = ∇·q = (q_right - q_left) / V
 * 
 * @param cells Cell array
 * @param ncell Number of cells
 * @param dt Current timestep
 */
#define apply_heat_conduction_source_terms(cells, ncell, dt) \
  _Pragma("omp parallel for") \
  for (uint_fast32_t i = 1; i < ncell + 1; ++i) { \
    /* Energy change due to heat conduction */ \
    const double heat_source = (cells[i]._heat_flux_right - cells[i]._heat_flux_left) / \
                              cells[i]._V; \
    \
    /* Update total energy */ \
    cells[i]._E += dt * heat_source; \
  }

/**
 * @brief Compute heat conduction timestep constraint.
 * 
 * Heat conduction imposes a stability constraint:
 * dt < 0.5 * dr² / α, where α = κ / (ρ * c_v) is thermal diffusivity
 * 
 * @param cells Cell array
 * @param ncell Number of cells
 * @return Minimum timestep due to heat conduction
 */
#define compute_heat_conduction_timestep(cells, ncell) \
  double min_heat_dt = 1e10; \
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
    min_heat_dt = std::min(min_heat_dt, heat_dt); \
  } \
  min_heat_dt

/**
 * @brief Initialize heat conduction variables.
 * 
 * Set heat fluxes to zero initially.
 * 
 * @param cells Cell array
 * @param ncell Number of cells
 */
#define initialize_heat_conduction(cells, ncell) \
  _Pragma("omp parallel for") \
  for (uint_fast32_t i = 0; i < ncell + 2; ++i) { \
    cells[i]._heat_flux_left = 0.; \
    cells[i]._heat_flux_right = 0.; \
  }

#else // HEAT_CONDUCTION == HEAT_CONDUCTION_OFF

/* Heat conduction disabled - do nothing */
#define compute_heat_conduction_fluxes(cells, ncell)
#define apply_heat_conduction_source_terms(cells, ncell, dt)
#define compute_heat_conduction_timestep(cells, ncell) 1e10
#define initialize_heat_conduction(cells, ncell)

#endif // HEAT_CONDUCTION == HEAT_CONDUCTION_ON

#endif // HEAT_CONDUCTION_HPP
