#!/bin/bash

SOURCE_DIR="$(dirname "$0")/.."

cd "$SOURCE_DIR"
git submodule update --init
git submodule update



cd "$SOURCE_DIR/submodules\DirectXShaderCompiler"
git submodule update --init
git submodule update