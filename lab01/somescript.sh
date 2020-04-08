#!/bin/bash
mkdir -p 8091/Skorodumov
cd 8091
cd Skorodumov
date > Sergei.txt
date --date="next mon" > filedata.txt
cat Sergei.txt filedata.txt > result.txt
cat result.txt
