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
 * @file OptionNames.hpp
 *
 * @brief Useful aliases for define options.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#ifndef OPTIONNAMES_HPP_
#define OPTIONNAMES_HPP_

// Possible types of boundary conditions.

/*! @brief Open boundaries: inflow or outflow depending on the local flow
 *  velocity */
enum class BoundaryType { OPEN, REFLECTIVE, BONDI };

enum class EOS { IDEAL, ISOTHERMAL };

enum class Potential { NONE, POINT_MASS };

enum class IC { UNIFORM, BONDI, READ };

enum class Riemann { EXACT, HLLC };

enum class Coordinate { CARTESIAN_1D, SPHERICAL_1D };

enum class HeatConduction { ON, OFF };

#endif  // OPTIONNAMES_HPP_
