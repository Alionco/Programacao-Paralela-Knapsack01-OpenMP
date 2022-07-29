#!/bin/bash

# grep 'real' 1/SAIDAS_LARGE_SCALE.txt | sed -E 's/[^0-9\.]+//g' | numsum 


ARQ_LARGE_SCALE="instances_01_KP/large_scale/*"

rm *.txt

gcc knapsack_dynamic.c -o dynamic -O3 -Wno-unused-result
gcc knapsack_parallel.c -o parallel -fopenmp -O3 -Wno-unused-result
gcc knapsack_naive.c -o naive -O3 -Wno-unused-result 

for ((c = 1; c <= 20; c++))
    do
    for arq in $ARQ_LARGE_SCALE
        do
        printf '**********************************' >> $c.txt
        echo $arq >> $c.txt
        printf '\n' >> $c.txt
        (time ./dynamic < $arq) >> $c.txt 2>&1
        printf '\n' >> $c.txt
        for ((i = 2; i <= 8; i = i * 2))
            do
            export OMP_NUM_THREADS=$i
            (time ./parallel < $arq) >> $c.txt 2>&1
            printf '\n' >> $c.txt
            done
        done
        printf '**********************************' >> $c.txt
    done

echo FIM