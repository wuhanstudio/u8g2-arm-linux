#!/bin/bash

# Clone repo and copy source files
git clone https://github.com/olikraus/u8g2
rm -rf ../csrc/*
cp u8g2/csrc/* ../csrc

# Generate C++ wrapper
cd codebuild && make install
