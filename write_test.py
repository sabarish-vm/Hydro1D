# write the configuration command for a heat conduction run

import get_cmake_command

heat_conduction_options = {
    "rmin_in_au": 10.0,
    "rmax_in_au": 5000.0,
    "ncell": 2000,
    "gamma":1.3,
    "maxtime_in_yr": 40.0,
    "number_of_snaps": 2000,
    "ic": "IC_BONDI",
    "eos": "EOS_BONDI",
    "boundaries": "BOUNDARIES_BONDI",
    "isothermal_temperature_in_k": 500.0,
    "potential": "POTENTIAL_POINT_MASS",
    "g_internal": 1.0,
    "mass_point_mass_in_msol": 18.0,
    "bondi_density_in_si": 1.0e-16,
    "bondi_pressure_contrast": 32.0,
    "unit_mass_in_si": 2.479e31,
    "unit_length_in_si": 1.2e13,
    "courant_factor": 0.05,
    "riemannsolver_type": "RIEMANNSOLVER_TYPE_HLLC",
    # Heat conduction parameters
    "heat_conduction": "HEAT_CONDUCTION_OFF",
    "thermal_conductivity": 0.0,
    "heat_conduction_timestep_factor": 0.1,
    "sound_infinity_in_si": 2030.71,
    "rho_infinity_in_si": 1e-16,
}

print(get_cmake_command.get_cmake_command(heat_conduction_options))
