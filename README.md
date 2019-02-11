# Know your mem

[![Build Status](https://travis-ci.com/o-o-overflow/dc2019q-know_your_mem.svg?token=6XM5nywRvLrMFwxAsXj3&branch=master)](https://travis-ci.com/o-o-overflow/dc2019q-know_your_mem)

Kinda classic: seccomp + find the flag page in memory.

One solution is to scan large chunks via mmap (hint where the memory should be, see if it was respected or not), then drill in to find the header.


## THERE'S A BRUTE-FORCE RISK

People may try to brute-force it "raw", or anyway scan inefficiently.

Since this is an easy-category challenge, I wrote a simplified version that can be done locally in C.
I hope this will help people find an efficient way to solve it.

Just for fun, I also made it work to compile directly to raw shellcode :)
