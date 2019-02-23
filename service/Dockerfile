from ubuntu:18.04
run apt-get update -qq && apt-get install -qq --no-install-recommends -y build-essential binutils python3 xinetd libseccomp-dev libseccomp2 bash

copy src/know_your_mem.c /
copy src/Makefile /
run make know_your_mem

run touch /flag && chmod 644 /flag && echo "OOO{so many bits, so many syscalls}" > /flag
copy know_your_mem.conf /know_your_mem.conf

# Other files that go in the public bundle
copy src/simplified.c /
copy src/simplified_shellcode.so.c /
copy src/shellcode.c /
copy src/topkt.py /
copy src/README.md /

expose 4669
cmd ["/usr/sbin/xinetd", "-filelog", "-", "-dontfork", "-f", "/know_your_mem.conf"]
# TODO: -filelog - doesn't work? /dev/stderr?
