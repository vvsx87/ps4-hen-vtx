#!/bin/bash

set -e

pushd tool
make
popd

pushd payload
make
popd

tool/bin2c payload/payload.bin > installer/source/payload_data.inc

pushd installer
make
popd

rm -f ps4-hen-vtx.bin
cp installer/installer.bin ps4-hen-vtx.bin

tool/bin2js ps4-hen-vtx.bin > exploit/payload.js
