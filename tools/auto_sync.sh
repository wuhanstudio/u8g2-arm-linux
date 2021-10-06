#!/bin/bash

# Clone repo and copy source files
git clone https://github.com/olikraus/u8g2
rm -rf ../csrc/*
cp u8g2/csrc/* ../csrc

cd codebuild

# Remove previous controller_list
sed -i '/\/\* display_controller_list_start \*\//,/\/\* display_controller_list_end \*\//{//!d}'  ./codebuild.c

# Get the latest controller_list
sed -n '/\/\* display_controller_list_start \*\//,/\/\* display_controller_list_end \*\//{p;}'  ../u8g2/tools/codebuild/codebuild.c | sed '1d;$d' > controller_list.txt
sed -i '/\/\* display_controller_list_start \*\// r controller_list.txt' ./codebuild.c
rm controller_list.txt

# Generate C++ wrapper
make install
