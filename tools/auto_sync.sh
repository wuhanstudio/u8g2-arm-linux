#!/bin/bash

# Clone repo and copy source files
git clone https://github.com/olikraus/u8g2
rm -rf ../csrc/*
cp u8g2/csrc/* ../csrc

# Generate C++ wrapper
cd codebuild
make
./codebuild
cp u8g2_d_setup.c u8g2_d_memory.c ../../csrc/

# Remove previous C++ headers
sed -i '/\/\* generated code (codebuild), u8g2 project \*\//,/#endif \/\/ U8X8_USE_PINS/{//!d}'  ../../cppsrc/U8g2lib.h
sed -i '/\/\* generated code (codebuild), u8g2 project \*\//,/#endif \/\/ U8X8_USE_PINS/{//!d}'  ../../cppsrc/U8x8lib.h

# Copy C++ headers into file 
sed -i '/\/\* generated code (codebuild), u8g2 project \*\// r U8g2lib.h' ../../cppsrc/U8g2lib.h
sed -i '/\/\* generated code (codebuild), u8g2 project \*\// r U8x8lib.h' ../../cppsrc/U8x8lib.h

# Remove the extra line /* generated code (codebuild), u8g2 project */
sed -i '0,/\/\* generated code (codebuild), u8g2 project \*\// s///' ../../cppsrc/U8g2lib.h
sed -i '0,/\/\* generated code (codebuild), u8g2 project \*\// s///' ../../cppsrc/U8x8lib.h
sed -i 'N;s/\n\/\* generated code (codebuild), u8g2 project \*\//\/\* generated code (codebuild), u8g2 project \*\//' ../../cppsrc/U8g2lib.h
sed -i 'N;s/\n\/\* generated code (codebuild), u8g2 project \*\//\/\* generated code (codebuild), u8g2 project \*\//' ../../cppsrc/U8x8lib.h
