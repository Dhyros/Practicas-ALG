#!/bin/bash

file="datos_tiempos.txt"
printf "" > $file

for ((i=500; i<=20000; i+=500)); do
	printf "N = $i\n"
	./script.sh 10 $i
	./Ejercicio1_exe pts.txt >> $file
done
