#!/usr/bin/env python

from PBN import Bignum

def factorial_bignum(num):
	if num <= 0:
		return Bignum(1)
	else:
		fact = Bignum(num)
		for i in range(1, num)[::-1]:
			fact = fact*Bignum(i)

		return fact

# Test here
a = Bignum(1241900)
b = Bignum(112314)
t = a + b
print(a.N(), "+", b.N(), "=", t.N())

c = Bignum("-100")
d = Bignum("3")
k = c + d
print(c.N(), "+", d.N(), "=", k.N())

t = a - b
print(a.N(), "-", b.N(), "=", t.N())

t = a * b
print(a.N(), "*", b.N(), "=", t.N())

k = c + d
print(c.N(), "+", d.N(), "=", k.N())

t = a - b
print(a.N(), "-", b.N(), "=", t.N())

t = a * b
print(a.N(), "*", b.N(), "=", t.N())

k = c * d
print(c.N(), "*", d.N(), "=", k.N())

t = a // b
print(a.N(), "//", b.N(), "=", t.N())

m = a % b
print(a.N(), "%", b.N(), "=", m.N())

k = c - d
print(c.N(), "-", d.N(), "=", k.N())

n = 130
fact = factorial_bignum(n)
print(n, "! = ", fact.N())
