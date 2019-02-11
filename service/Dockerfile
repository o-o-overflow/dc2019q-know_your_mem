from ubuntu:18.04
run echo "travis_fold:start:Dapt\033[33;1mservice Dockerfile apt\033[0m" && \
    apt-get update -qq && apt-get install -qq --no-install-recommends -y build-essential binutils python3 xinetd libseccomp-dev libseccomp2 bash && \
    echo "\ntravis_fold:end:Dapt\r"

copy src/know_your_mem /
#copy src/know_your_mem.c /
#copy src/Makefile /
# Disabled building, to ensure the public bundle and the container have the same binary
#run make know_your_mem

run touch /flag && chmod 644 /flag && echo "OOO{yeah well, still easy}" > /flag
copy know_your_mem.conf /know_your_mem.conf

copy banner_fail /

expose 4669
cmd ["/usr/sbin/xinetd", "-filelog", "-", "-dontfork", "-f", "/know_your_mem.conf"]
# TODO: -filelog - doesn't work? /dev/stderr?
