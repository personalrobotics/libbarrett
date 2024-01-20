#!/usr/bin/env bash

# Go to libbarrett base directory
BASE_DIR=$(git rev-parse --show-toplevel)
if [[ ${BASE_DIR} != *"libbarrett"* ]]; then
  echo "Cannot run from outside libbarrett directory."
  exit 10;
fi

cd $BASE_DIR

# Remove CMake generated files
rm -rf CMakeCache.txt                 \
       CMakeFiles/                    \
       cmake_install.cmake            \
       cmake/barrett-config.cmake     \
       install_manifest.txt           \
       Makefile
