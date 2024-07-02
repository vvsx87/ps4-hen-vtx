#!/bin/bash

pushd kpayload
make clean
popd

pushd installer
make clean
popd

rm -f ps4-hen-1070-1071-PPPwn-vtx.bin payload.js
