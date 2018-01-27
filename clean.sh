#!/bin/bash

pushd tool
make clean
popd

pushd payload
make clean
popd

pushd installer
make clean
popd

rm -f ps4-hen-vtx.bin
