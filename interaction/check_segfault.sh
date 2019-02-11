#!/bin/bash -e
NAME=segfault
make -s -C src "checks/$NAME.bin.pkt"
nc "$1" "$2" < "src/checks/$NAME.bin.pkt" || true
