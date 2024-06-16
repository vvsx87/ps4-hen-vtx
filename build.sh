#!/bin/bash

set -e

pushd kpayload
make
popd

pushd installer
make
popd

rm -f ps4-hen-755-PPPwn-vtx.bin
cp installer/installer.bin ps4-hen-755-PPPwn-vtx.bin
