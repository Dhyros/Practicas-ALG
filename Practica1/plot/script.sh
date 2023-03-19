#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Error: Número de argumentos incorrecto."
    echo "Uso: ${0} <ejercicio>"
    echo
    echo "Ejercicios disponibles:"
    for file in `find .. -iname "ej*.cpp"`; do
        echo `cut -c 4-$((${#file}-4)) <<< $file`
    done
    exit -1
fi

ej="../${1}"
file="${1}.dat"
printf "" > $file

#############################################################################
#                              COMPILACION                                  #
#############################################################################

g++ "${ej}.cpp" -o "${ej}_exe"
chmod 777 "${ej}_exe"

#############################################################################
#                          RECOPILACION DE DATOS                            #
#############################################################################

ej="${ej}_exe"
for ((i=1000; i<=30000; i+=1000)); do
    echo "N1 = ${i}"
    $ej $i $RANDOM >> $file
done

#############################################################################
#                               GRAFICAS                                    #
#############################################################################

gnuplot -c grafica.p $file "$1.png"
