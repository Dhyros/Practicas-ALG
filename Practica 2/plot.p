set terminal png

set output "graficas.png"

set size ratio 1

plot "puntos.txt", "sorted_sol.txt" with lines