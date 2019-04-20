from ubuntu:18.04
run echo "travis_fold:start:DIapt\033[33;1minteractions Dockerfile pkgs\033[0m" && \
    apt-get -qq update && apt-get -qq install --no-install-recommends -y build-essential binutils python3 xinetd libseccomp-dev libseccomp2 bash netcat-openbsd && \
    echo "\ntravis_fold:end:DIapt\r"
#COPY requirements.txt /requirements.txt
#RUN pip install -r requirements.txt

ENV TERM linux

#COPY *.py /
COPY *.sh /
COPY src/ /src/

# Builds what's not checked-in (simplified+checks) + self-tests
run make -C src mycheck 2>/dev/null
