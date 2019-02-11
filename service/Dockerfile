from ubuntu:18.04
run apt-get update && apt-get install -y build-essential binutils python3 xinetd libseccomp-dev libseccomp2 bash

copy src/know_your_mem.c /
copy src/Makefile /
run make know_your_mem

run touch /flag && chmod 644 /flag && echo "OOO{this is a test flag}" > /flag
copy know_your_mem.conf /know_your_mem.conf
copy wrapper /wrapper

expose 4669
cmd ["/usr/sbin/xinetd", "-filelog", "-", "-dontfork", "-f", "/know_your_mem.conf"]
