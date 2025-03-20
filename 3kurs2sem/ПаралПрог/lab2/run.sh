gcc test.c -pthread -lm -o t.o 

if [ -f ./t.o ]
then
time ./t.o 2
time taskset -c 0 ./t.o 1
rm t.o

else
echo compile error

fi