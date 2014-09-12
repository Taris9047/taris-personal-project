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
		lst_A = self.lstN
		lst_B = other.lstN
		carry = 0

		# TODO: Implement addition algorithm
		

			
		return bnum_answer

	def full_adder(an, bn, cn):
		rn = an + bn + cn

		if rn > 10:
			cn_out = rn/10;
			rn = rn%10;
		else:
			cn_out = 0;

		return rn, cn_out

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


