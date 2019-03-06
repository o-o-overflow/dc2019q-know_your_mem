from ubuntu:18.04
run apt-get update -qq && apt-get install -qq --no-install-recommends -y build-essential binutils python3 xinetd libseccomp-dev libseccomp2 bash

copy src/know_your_mem.c /
copy src/know_your_mem /
copy src/Makefile /
# Disabled building, to ensure the public bundle and the container have the same binary
#run make know_your_mem

run touch /flag && chmod 644 /flag && echo "OOO{so many bits, so many syscalls}" > /flag
copy know_your_mem.conf /know_your_mem.conf

expose 4669
cmd ["/usr/sbin/xinetd", "-filelog", "-", "-dontfork", "-f", "/know_your_mem.conf"]
# TODO: -filelog - doesn't work? /dev/stderr?
