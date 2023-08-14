#!/usr/bin/env bash
Inputs=( 16 32 64 128 256 512 1024)
Palettes=( 16 32 64 128 )
for j in "${Palettes[@]}"; do
    for i in "${Inputs[@]}"; do
        echo "--$j $i--" 1>> results_new.txt
        { time ./color_quantization $i 1 $j; } 2>> results_new.txt
    done
done
