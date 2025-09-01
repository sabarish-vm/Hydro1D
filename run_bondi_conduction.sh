#! /bin/bash

# run a Bondi setup simulation with heat conduction in the folder given below, with the number of
# threads set below

folder=build_bondi_conduction
nthread=8

cmake_command=$(python write_configuration_heat_conduction.py)

mkdir $folder
cd $folder
echo $cmake_command
eval $cmake_command
make -j $nthread
OMP_NUM_THREADS=$nthread OMP_PROC_BIND=True ./HydroCodeSpherical1D 2>&1 |
    tee bondi_conduction.log
cd ..
