#!/bin/bash

file="pts.txt"

if [ $# -ne 2 ]; then
    printf "ERROR: Invalid number of arguments\n"
    printf "Usage: %s <dimension> <amount of points>\n" $0
    exit -1
elif [ $1 -le 1 ]; then
    printf "ERROR: Invalid dimension\n"
    printf "Dimension must be greater or equal to 2\n"
    exit -1
fi

#############################################################################
# COMPILATIONN

make

#############################################################################
# EXECUTION

printf "Generating points ... "

# Dimension and amount of points
printf "$1 $2\n\n" > $file

# Number of elemnts
for ((i=0; i<$2; i++)); do
    # Insertion of points
    Arr=()
    for ((j=0; j<$1; j++)); do
        Arr+=($(($RANDOM%100)))
    done
    printf "${Arr[*]}\n" >> $file
done
printf "Points generated\n"


#############################################################################
# Executions
#############################################################################

./Ejercicio1_exe pts.txt > sol.txt

n=`cat pts.txt | wc -l`
n=$((n-2))
tail -n$n pts.txt > puntos.txt

sort sol.txt > sorted_sol.txt

gnuplot plot.p

rm pts.txt sol.txt