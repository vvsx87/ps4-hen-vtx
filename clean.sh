#!/bin/bash

pushd kpayload
make clean
popd

pushd installer
make clean
popd

rm -f ps4-hen-950-PPPwn-vtx.bin payload.js
