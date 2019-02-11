# sudo apt-get install libseccomp-dev libseccomp2

CFLAGS ?= -Wall

# 64 bit!
CFLAGS += -m64 -std=gnu99

# Just making sure
HARDENING_FLAGS += -pie -fPIE -Wl,-z,relro -Wl,-z,now -fstack-protector-all -D_FORTIFY_SOURCE=2 -Wformat=2 -Werror=format-security


all: know_your_mem


know_your_mem: LDLIBS += -lseccomp
know_your_mem: CFLAGS += -O3 -march=native -s $(HARDENING_FLAGS)
#know_your_mem: CFLAGS += -O3 -march=native -g $(HARDENING_FLAGS)
know_your_mem: know_your_mem.c jump_to_shellcode.s


%.so: CFLAGS += -shared -fPIC
%.so: LDFLAGS += -shared -fPIC


check: all flag shellcode.bin.pkt
	./know_your_mem < shellcode.bin.pkt | tee | fgrep --text 'OOO{theflagwillbehere}'
	@echo "Perfect! Now go get that flag :)"


mycheck: all flag solution.bin.pkt
	./know_your_mem < solution.bin.pkt | tee | fgrep --text 'OOO{theflagwillbehere}'
	@echo "Good, my solution worked :)"


%.bin: %.c
	gcc -nostdlib -static -fPIC -Os -Wall -DNDEBUG -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables -s -o $*.elf $<
	!(readelf -W --sections $*.elf | egrep '\.(ro)?data') || echo -e "\n\nWARNING: you have .(ro)data, you'll have to adjust this build.\n\n" >/dev/stderr
	objcopy $*.elf --dump-section .text=$@

%.bin.pkt: %.bin
	./topkt.py $<


flag:
	@echo 'Creating a fake flag for testing convenience'
	@echo 'OOO{theflagwillbehere} Make sure you print it to stdout, stderr may go to /dev/null in the hosted version.' > flag

.PHONY: clean all check mycheck
clean:
	rm -f know_your_mem shellcode.bin shellcode.bin.pkt solution.bin solution.bin.pkt checks/*.pkt checks/*.elf *.elf *.so
