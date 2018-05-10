#!/bin/bash
HOST=x86_64-w64-mingw32
CXX=x86_64-w64-mingw32-g++-posix
CC=x86_64-w64-mingw32-gcc-posix
PREFIX="$(pwd)/depends/$HOST"

set -eu -o pipefail

set -x
cd "$(dirname "$(readlink -f "$0")")/.."

cd depends/ && make HOST=$HOST V=1 NO_QT=1 && cd ../
./autogen.sh
sed -i 's/-lboost_system-mt /-lboost_system-mt-s /' configure
sed -i 's/-lboost_program_options-mt /-lboost_program_options-mt-s /' configure
CXXFLAGS="-DPTW32_STATIC_LIB -DCURVE_ALT_BN128 -fopenmp -pthread" CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --prefix="${PREFIX}" --host=x86_64-w64-mingw32 --enable-static --disable-shared --disable-rust --disable-proton
cd src/
CC="${CC}" CXX="${CXX}" make V=1 -j4 hushd.exe hush-cli.exe hush-tx.exe
