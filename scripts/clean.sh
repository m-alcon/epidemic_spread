#!/bin/bash

if [ -n "$1" ]; then
    rm data/sir/*
    rm data/sis/*
    rm data/sir/00/*
    rm data/sis/00/*
    rm data/sir/01/*
    rm data/sis/01/*
    rm data/sir/02/*
    rm data/sis/02/*
    rm data/sir/10/*
    rm data/sis/10/*
    rm data/sir/11/*
    rm data/sis/11/*
    rm data/sir/12/*
    rm data/sis/12/*
fi

cd src
make clean

