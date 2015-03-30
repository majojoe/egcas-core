#/bin/bash -e

if [ -d build ]; then
        rm -rf build
fi

EGCAS_PKG_PATH="${PWD}"
LIBMMLEGCAS_PKG_PATH="${PWD}../../../libmml_egcas/library/"

mkdir build
export LD_LIBRARY_PATH="LD_LIBRARY_PATH:${LIBMMLEGCAS_PKG_PATH}"
cd "${LIBMMLEGCAS_PKG_PATH}"
if [ -d build ]; then
        rm -rf build
fi
mkdir build
cd build
cmake ..
make -j8
cd "${EGCAS_PKG_PATH}"
cd build
cmake ..
make -j8 package
mv *.deb ../../../..
cd ..
rm -rf build
rm -rf "${LIBMMLEGCAS_PKG_PATH}/build"