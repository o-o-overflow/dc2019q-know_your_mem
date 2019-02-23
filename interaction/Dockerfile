from ubuntu:18.04
run apt-get -qq update && apt-get -qq install --no-install-recommends -y build-essential binutils python3 xinetd libseccomp-dev libseccomp2 bash netcat-openbsd
#COPY requirements.txt /requirements.txt
#RUN pip install -r requirements.txt

ENV TERM linux

#COPY *.py /
COPY *.sh /
COPY src/ /src/

# Builds everything + self-tests
run make -C src mycheck 2>/dev/null
