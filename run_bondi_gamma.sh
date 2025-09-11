#! /bin/bash

# run a Bondi setup simulation with heat conduction in the folder given below, with the number of
# threads set below

GAMMA="$1"
GAMMA_NAME="${GAMMA/./p}"
folder=build_gamma_${GAMMA_NAME}
nthread=8

echo "GAMMA = ${GAMMA}"
echo "GAMMA_NAME = ${GAMMA_NAME}"
sed -i -E "s#\"gamma\":.*#\"gamma\":${GAMMA},#" ./write_test.py
cmake_command=$(python write_test.py)

echo "$cmake_command"

mkdir $folder
cd $folder
echo $cmake_command
eval $cmake_command
make -j $nthread
OMP_NUM_THREADS=$nthread OMP_PROC_BIND=True ./HydroCodeSpherical1D 2>&1 |
    tee bondi_conduction.log
cd ..
