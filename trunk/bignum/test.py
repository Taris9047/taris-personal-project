#!/usr/bin/env python

from PBN import Bignum

# Test here
a = Bignum(1241900)
b = Bignum(112314)
t = a + b
print a.N(), "+", b.N(), "=", t.N()

c = Bignum("-1000")
d = Bignum("-34989089")
k = c + d
print c.N(), "+", d.N(), "=", k.N()