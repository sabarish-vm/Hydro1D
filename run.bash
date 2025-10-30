#!/usr/bin/env bash
set -e
trap 'echo; echo "Use -h to see help">&2' ERR

while [[ "$#" -gt 0 ]]; do
    case "$1" in
    -i)
        inputfile=$2
        shift 2
        ;;
    -d)
        buildDir=$2
        shift 2
        ;;
    -j)
        nthread=$2
        shift 2
        ;;
    -h)
        echo "-i <inputfile.yml>"
        echo "-d <build directory>"
        echo "-j <nthreads>"
        exit 0
        ;;
    *)
        shift 1
        ;;
    esac
done
nthread=${nthread:-8}
cmake_command=$(python ./get_cmake_command.py "$inputfile")

mkdir -p $buildDir
cp "$inputfile" "${buildDir}/"
cd $buildDir
cmake_command="${cmake_command} -S .. -B ."
echo
echo
eval "${cmake_command}"
make -j "$nthread"
OMP_NUM_THREADS=$nthread OMP_PROC_BIND=True ./HydroCodeSpherical1D 2>&1 |
    tee bondi_conduction.log
cd ..
