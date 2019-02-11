#/bin/bash
# XXX
set -eux -o pipefail
rm -rf interaction/src/
cp -a service/src interaction/src
