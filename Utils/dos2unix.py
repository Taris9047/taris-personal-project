#!/usr/bin/env python
"""
convert dos linefeeds (cr/lf) to unix (lf)
usage: dos2unix.py <input>
"""

import sys

if len(sys.argv[1:]) < 1:
    print("Convert DOS (CR/LF) to UNIX (LF)")
    print("Usage: dos2unix.py <input>")
    sys.exit(__doc__)

content = ''
outsize = 0

with open(sys.argv[1], 'rb') as inf:
    content = inf.read();

outfn = sys.argv[1]

with open(outfn, 'wb') as outf:
    for line in content.splitlines():
        outsize += len(line) + 1
        outf.write(line+'\n')

print("Done, Saved %s bytes." % (len(content)-outsize))

