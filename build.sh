#!/bin/bash
unamestr=`uname`
if [[ "$unamestr" == 'Darwin' ]]; then
	ninja='input/ninja/ninja'
else
	ninja='ninja'
fi
set -e
pushd whitgl
python build.py
$ninja
popd
python build.py
$ninja
