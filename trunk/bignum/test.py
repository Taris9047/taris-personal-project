#!/usr/bin/env python

from PBN import Bignum

# Test here
a = Bignum(1241900)
b = Bignum(112314)
t = a + b
print a.N(), "+", b.N(), "=", t.N()

c = Bignum("100")
d = Bignum("-3")
k = c + d
print c.N(), "+", d.N(), "=", k.N()