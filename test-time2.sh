#!/usr/bin/env bash

Inputs=( 16 32 64 128 )
for i in "${Inputs[@]}"; do
    echo "--sequential $i--" 1>> results_new.txt
    { time ./color_quantization 1 1 $i; } 2>> results_new.txt
done
