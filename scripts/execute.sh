#!/bin/bash

k=4
l=8
lambda_k=1.4
lambda_l=0.89
max=22
delta=2

SECONDS=0
echo "============ 00 ============"
mkdir -p data/sir/00
mkdir -p data/sis/00
./bin/epidemic.exe 0 $k 0 $l
echo "============ 01 ============"
mkdir -p data/sir/01
mkdir -p data/sis/01
./bin/epidemic.exe 0 $k 1 $lambda_l $max
echo "============ 02 ============"
mkdir -p data/sir/02
mkdir -p data/sis/02
./bin/epidemic.exe 0 $k 2 $delta
echo "============ 10 ============"
mkdir -p data/sir/10
mkdir -p data/sis/10
./bin/epidemic.exe 1 $lambda_k $max 0 $l
echo "============ 11 ============"
mkdir -p data/sir/11
mkdir -p data/sis/11
./bin/epidemic.exe 1 $lambda_k $max 1 $lambda_l $max
echo "============ 12 ============"
mkdir -p data/sir/12
mkdir -p data/sis/12
./bin/epidemic.exe 1 $lambda_k $max 2 $delta

echo "Time: $SECONDS"