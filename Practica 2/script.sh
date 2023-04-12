#!/bin/bash

if [ $# -ne 1 ]; then
    printf "ERROR: Invalid number of arguments\n"
    printf "Usage: %s <amount of points>\n" $0
    exit -1
fi

#############################################################################
#                               COMPILATION                                 #
#############################################################################

g++ generador_puntos.cpp -o generador_puntos_exe

g++ Practica2.cpp -o Practica2_exe

#############################################################################
#                                EJECUCIÓN                                  #
#############################################################################

printf "Generando puntos ... "
./generador_puntos_exe $1
printf "Puntos generados\n"

