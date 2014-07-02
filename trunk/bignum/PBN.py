#!/usr/bin/env python

#
# Python Bignum Library, written by Taylor Shin. 
#

class Bignum:
	""" Initialization """
	def __init__(self, N = []):
		self.lstN = self.int_to_list(N)
	def int_to_list(self, N):
		""" Save a given integer into a list. """
		if N == []:
			self.sign = ''
			return []
		if isinstance(N,int) != True:
			raise ValueError("N is not an integer!! Program Bug!!")
		else:
			if N < 0:
				self.sign = '-'
				N *= -1
				lstN = [int(i) for i in str(N)]
			else:
				self.sign = ''
				lstN = [int(i) for i in str(N)]

			return lstN

	""" Calculation!! """
	def __add__(self, other):
		# TODO: Implement ++, +-, -+, -- situations

		bnum_answer = Bignum()
		ans_list = []
		lst_A = self.lstN
		lst_B = other.lstN
		print lst_A, lst_B
		carry = 0

		# TODO: Implement addition algorithm
		# remark 1: different list length needs to be handled.
		# remark 2: situation above needs to be handled as well.

			
		return bnum_answer 

	""" Utilities """
	def print_N(self):
		""" Prints the lstN into integer format. """
		if self.sign == '-':
			self.lstN = [x for x in self.lstN]
		print self.sign+''.join(str(x) for x in self.lstN)
	
	def return_N(self):
		if self.sign == '-':
			self.lstN = [x for x in self.lstN]
		""" Returns lstN into an integer. """
		return int(self.sign+''.join(str(x) for x in self.lstN))


# Test here
a = Bignum(1241900)
b = Bignum(-112314)

print "bignum a is: ", a.print_N()
print "bignum b is: ", b.print_N()
t = a + b
t.print_N()