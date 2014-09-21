#!/usr/bin/env python

#
# Python Bignum Library, written by Taylor Shin. 
#

class Bignum:
	""" Initialization """
	def __init__(self, N = []):
		# Constructing from Integer input
		if isinstance(N, int):
			self.lstN, self.sign = self.int_to_list(N)

		# Constructing from List input
		elif isinstance(N, list):
			if N == []:
				self.sign = ''
				self.lstN = [0]
			else:
				if N[0] == '-':
					self.sign = '-'
					del N[0]
				else:
					self.sign = ''

				if N != []:
					self.lstN = [int(i) for i in N]
				else:
					self.lstN = [0]

		# Constructing from String input
		elif isinstance(N, str):
			if N == "":
				self.sign = ''
				self.lstN = [0]
			else:
				if N[0] == '-':
					self.sign = '-'
					N = N[1:]
				else:
					self.sign = ''

				if N != '':
					self.lstN = [int(i) for i in N]
				else:
					self.lstN = [0]


	def int_to_list(self, N):
		""" Save a given integer into a list. """
		if N == []:
			sign = ''
			return []
		if isinstance(N,int) != True:
			raise ValueError("N is not an integer!! Program Bug!!")
		else:
			if N < 0:
				sign = '-'
				N *= -1
				lstN = [int(i) for i in str(N)]
			else:
				sign = ''
				lstN = [int(i) for i in str(N)]

			return lstN, sign

	""" 
	Operator Overloading 
	"""
	# +
	def __add__(self, other):
		bnum_answer = Bignum()
		# Positive + Positive
		if self.sign == '' and other.sign == '':
			bnum_answer.sign = ''
			bnum_answer.lstN = self.add(self.lstN, other.lstN)
			return bnum_answer
		# Negative + Negative
		elif self.sign == '-' and other.sign == '-':
			bnum_answer.sign = '-'
			bnum_answer.lstN = self.add(self.lstN, other.lstN)
			return bnum_answer
		# Positive + Negative

		# Negative + Positive

	# ==
	def __eq__(self, other):
		if self.sign != other.sign:
			return False
		else:
			

	""" Adding Positive + Positive """
	def add(self, A, B):
		if isinstance(A, list) != True:
			raise ValueError("add, A is not a list!!")
		if isinstance(B, list) != True:
			raise ValueError("add, B is not a list!!")

		cn = 0;
		indexA = len(A) - 1
		indexB = len(B) - 1

		indexMain = max(indexA, indexB)
		ret = [0]*max(len(A), len(B))

		for i in reversed(range(len(ret))):
			if indexA >= 0 and indexB >= 0:
				ret[indexMain], cn = self.full_adder(A[indexA], B[indexB], cn)
			elif indexA >= 0 and indexB < 0:
				ret[indexMain], cn = self.full_adder(A[indexA], 0, cn)
			elif indexB >= 0 and indexA < 0:
				ret[indexMain], cn = self.full_adder(0, B[indexB], cn)
			else:
				break

			indexA -= 1
			indexB -= 1
			indexMain -= 1

		if cn != 0:
			ret = [cn] + ret

		return ret

	""" Subtracting Big - Small positives """
	def sub(self, A, B):
		if isinstance(A, list) != True:
			raise ValueError("add, A is not a list!!")
		if isinstance(B, list) != True:
			raise ValueError("add, B is not a list!!")


	"""

	a full adder: just adds A, B, and Carry and returns its result and
	the next Carry

	"""
	def full_adder(self, an, bn, cn):
		rn = an + bn + cn

		if rn >= 10:
			cn_out = rn/10;
			rn = rn%10;
		else:
			cn_out = 0;

		return rn, cn_out

	""" Utilities """
	def print_N(self):
		""" Prints the lstN into integer format. """
		print self.sign+''.join(str(x) for x in self.lstN)
	
	def N(self):
		if self.sign == '-':
			self.lstN = [x for x in self.lstN]
		""" Returns lstN into an integer. """
		return int(self.sign+''.join(str(x) for x in self.lstN))


