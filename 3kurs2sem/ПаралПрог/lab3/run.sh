mpicc -fopenmp main.c -o main
mpirun -np 4 ./main 500000 u
rm main