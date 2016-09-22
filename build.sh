#!/bin/bash

./clean.sh
cmake -H. -Bbuild
cd build && make