#!/usr/bin/env bash
set -euo pipefail

PREFIX="${1:-/opt/cross}"
TARGET=x86_64-elf
BINUTILS_VER=2.42
GCC_VER=14.1.0
JOBS="${JOBS:-$(nproc)}"

mkdir -p "$PREFIX" /tmp/aetheros-toolchain
cd /tmp/aetheros-toolchain

if [ ! -f "binutils-${BINUTILS_VER}.tar.xz" ]; then
  curl -LO "https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.xz"
fi
if [ ! -f "gcc-${GCC_VER}.tar.xz" ]; then
  curl -LO "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.xz"
fi

tar -xf "binutils-${BINUTILS_VER}.tar.xz"
tar -xf "gcc-${GCC_VER}.tar.xz"

mkdir -p build-binutils build-gcc

cd build-binutils
../binutils-${BINUTILS_VER}/configure --target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j"$JOBS"
make install

cd ../gcc-${GCC_VER}
./contrib/download_prerequisites

cd ../build-gcc
../gcc-${GCC_VER}/configure --target="$TARGET" --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
make -j"$JOBS" all-gcc all-target-libgcc
make install-gcc install-target-libgcc

echo "Toolchain installed to $PREFIX"
echo "Add to PATH: export PATH=$PREFIX/bin:\$PATH"
