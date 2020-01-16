#!/bin/bash

if [ -n "$1" ]; then
    rm data/sir/*
    rm data/sis/*
fi

cd src
make clean

