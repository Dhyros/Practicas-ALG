#!/bin/bash

file="datos_tiempos.txt"
printf "" > $file

for ((i=1000; i<=50000; i+=1000)); do
	printf "N = $i\n"
	./script.sh 10 $i
	./Ejercicio1_exe pts.txt >> $file
done
