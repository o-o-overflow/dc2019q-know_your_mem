from ubuntu:18.04
run apt-get update && apt-get install -y build-essential binutils python3 xinetd libseccomp-dev libseccomp2 bash netcat-openbsd
#COPY requirements.txt /requirements.txt
#RUN pip install -r requirements.txt

ENV TERM linux

#COPY *.py /
COPY *.sh /
COPY src/ /src/

# Builds everything + self-tests
run make -C src mycheck
