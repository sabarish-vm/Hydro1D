# write the configuration command for a heat conduction run

import get_cmake_command

heat_conduction_options = {
    "rmin_in_au": 10.0,
    "rmax_in_au": 5000.0,
    "ncell": 2000,
    "gamma":1.5,
    "maxtime_in_yr": 40.0,
    "number_of_snaps": 2000,
    "potential": "Potential::POINT_MASS",
    "mass_point_mass_in_msol": 10.0,
    "bondi_density_in_si": 1.0e-15,
    "unit_mass_in_si": 1.98840987069805e31,
    "unit_length_in_si": 2.7091640467030e15,
    "unit_time_in_si": 2.70916404670302e11,
    "courant_factor": 0.05,
    "riemannsolver_type": "Riemann::HLLC",
    "sound_infinity_in_si": 10000,
    "rho_infinity_in_si": 1e-15,
    "velocity_infinity_in_si": 1,
    "coord_system": "Coordinate::SPHERICAL_1D",
}

print(get_cmake_command.get_cmake_command(heat_conduction_options))
