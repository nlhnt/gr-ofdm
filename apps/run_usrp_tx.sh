#!/bin/sh

cd $(dirname $0)

echo "$*"

if [ -d "$PWD/../build" ]; then
    . $PWD/environment_debug
else
    . $PWD/environment_release
fi

tx.py -f 2.45G --bandwidth=1M --fft-length=256 --subcarriers=200 $*

