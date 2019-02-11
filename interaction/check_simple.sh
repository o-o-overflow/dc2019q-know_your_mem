#!/bin/bash -e
NAME=simple
make -C src "checks/$NAME.bin.pkt"
nc "$1" "$2" < "src/checks/$NAME.bin.pkt" | grep "Ciao"
