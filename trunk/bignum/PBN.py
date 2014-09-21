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
		elif self.sign == '' and other.sign == '-':
			if self.lstN > other.lstN[1:]:
				bnum_answer.sign = ''
				bnum_answer.lstN = self.sub(self.lstN, other.lstN)
				return bnum_answer
			else:
				bnum_answer.sign = '-'
				bnum_answer.lstN = self.sub(other.lstN, self.lstN)
				return bnum_answer

		# Negative + Positive

	# ==
	def __eq__(self, other):
		if self.sign != other.sign:
			return False
		else:
			return self.lstN == other.lstN



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

		indexA = len(A) - 1
		indexB = len(B) - 1
		indexMain = max(indexA, indexB)
		ret = [0]*max(len(A), len(B))

		for i in range(1,len(ret)):
			if min(indexA, indexB) > 0:
				ret[indexMain], A[indexA-1] = self.subtractor(A[indexA], B[indexB], A[indexA-1])
			else:
				ret[indexMain] = A[indexA]

			#print A[indexA], B[indexB], ret[indexMain]

			indexA -= 1
			indexB -= 1
			indexMain -= 1

			#print "ret[", indexMain, "]: ", ret[indexMain]

#		for i in range(0,len(ret)):
#			if ret[i] == 0:
#				del ret[i]
#
#			elif ret[i] == 0 and i == len(ret) - 1:
#				break
#			else:
#				break

		return ret


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

	"""

	full_subtractor: subtracts with A, B and borrow digit and return its result

	"""
	def subtractor(self, an, bn, borrow_digit):
		if an < bn:
			rn = 10 + an - bn
			return rn
			if borrow_digit == 0:
				borrow_digit = 9
			else:
				borrow_digit -= 1
		else:
			rn = an - bn

		return rn, borrow_digit

	"""
	borrow: manipulate digits after borrowing at a certain location

	"""
	def borrow(self, A, position):
		if position < 0:
			position = 0
		elif position > len(A)-1:
			position = len(A) - 1

		if A[i] == 0:
			for i in reversed(range(0, position)):
				if A[i] == 0:
					A[i] == 9
				elif A[i] > 0:
					A[i] -= 1
					break

		if A[0] == 0:
			del A[0]

		return A


	""" Utilities """
	def print_N(self):
		""" Prints the lstN into integer format. """
		print self.sign+''.join(str(x) for x in self.lstN)
	
	def N(self):
		if self.sign == '-':
			self.lstN = [x for x in self.lstN]
		""" Returns lstN into an integer. """
		return int(self.sign+''.join(str(x) for x in self.lstN))


