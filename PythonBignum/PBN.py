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
		# Negative + Negative
		elif self.sign == '-' and other.sign == '-':
			bnum_answer.sign = '-'
			bnum_answer.lstN = self.add(self.lstN, other.lstN)
		# Positive + Negative
		elif self.sign == '' and other.sign == '-':
			if self.lstN > other.lstN[1:]:
				bnum_answer.sign = ''
				bnum_answer.lstN = self.sub(self.lstN, other.lstN)
			else:
				bnum_answer.sign = '-'
				bnum_answer.lstN = self.sub(other.lstN, self.lstN)
		# Negative + Positive
		elif self.sign == '-' and other.sign == '':
			if self.lstN > other.lstN[1:]:
				bnum_answer.sign = '-'
				bnum_answer.lstN = self.sub(self.lstN, other.lstN)
			else:
				bnum_answer.sign = ''
				bnum_answer.lstN = self.sub(other.lstN, self.lstN)

		return bnum_answer

	# -
	def __sub__(self, other):
		bnum_answer = Bignum()
		# Positive - Negative
		if self.sign == '' and other.sign == '-':
			bnum_answer.sign = ''
			bnum_answer.lstN = self.add(self.lstN, other.lstN)
		# Negative - Positive
		elif self.sign == '-' and other.sign == '':
			bnum_answer.sign = '-'
			bnum_answer.lstN = self.add(self.lstN, other.lstN)
		# Positive - Positive
		elif self.sign == '' and other.sign == '':
			if self.lstN > other.lstN:
				bnum_answer.sign = ''
				bnum_answer.lstN = self.sub(self.lstN, other.lstN)
			else:
				bnum_answer.sign = '-'
				bnum_answer.lstN = self.sub(other.lstN, self.lstN)
		elif self.sign == '-' and other.sign == '-':
			if self.lstN > other.lstN:
				bnum_answer.sign = '-'
				bnum_answer.lstN = self.sub(other.lstN, self.lstN)
			else:
				bnum_answer.sign = ''
				bnum_answer.lstN = self.sub(self.lstN, other.lstN)

		return bnum_answer

	# *
	def __mul__(self, other):
		answer = Bignum()
		if self.sign == other.sign:
			answer.sign = ''
		else:
			answer.sign = '-'

		answer.lstN = self.mul(self.lstN, other.lstN)

		return answer

	# //
	def __floordiv__(self, other):
		answer = Bignum()
		if self.sign == other.sign:
			answer.sign = ''
		else:
			answer.sign = '-'

		answer.lstN = self.floordiv(self.lstN, other.lstN)

		return answer

	# %
	def __mod__(self, other):
		answer = Bignum()
		if self.sign == other.sign:
			answer.sign = ''
		else:
			answer.sign = '-'

		answer.lstN = self.moddiv(self.lstN, other.lstN)

		return answer

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

		for i in reversed(list(range(len(ret)))):
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
		ret = [0]*len(A)
		
		borrow = 0
		for i in range(0, len(ret)):
			if indexB >= 0 and indexA >= 0:
				an = A[indexA]
				bn = B[indexB]
				rn, borrow = self.subtractor(an, bn, borrow)
				ret[indexMain] = rn
			elif indexB < 0 and indexA >= 0:
				an = A[indexA]
				bn = 0
				rn, borrow = self.subtractor(an, bn, borrow)
				ret[indexMain] = rn
			else:
				break

			indexA -= 1
			indexB -= 1
			indexMain -= 1

		# Trim zeros from ret
		indexMain = 0
		for i in range(0, len(ret)):
			if ret[indexMain] == 0:
				ret = ret[1:]
				indexMain -= 1
			else:
				break

		return ret

	""" Multiplication """
	def mul(self, A, B):
		if isinstance(A, list) != True:
			raise ValueError("add, A is not a list!!")
		if isinstance(B, list) != True:
			raise ValueError("add, B is not a list!!")

		answer = [0]*len(A)
		#index = [0]

		# Return [0] if one of lists are [0]
		if A == [0] or B == [0]:
			return [0]

		# Performing addition
		'''
		while True:
			answer = self.add(answer, A)
			index = self.add(index, [1])
			if index == B:
				break

		del index
		'''

		indexB = 0
		while True:
			if (indexB == len(B)):
				break

			mul_residual = [0]*len(A)
			carry = 0
			for i in range(len(A)):
				rn = A[i] * B[indexB] + carry
				carry = rn//10
				mul_residual[i] = rn%10

			if carry != 0:
				mul_residual = [carry] + mul_residual

			mul_residual = mul_residual+([0]*indexB)
			#print mul_residual
			answer = self.add(mul_residual, answer)

			indexB += 1

		return answer


	""" Division (floor) """
	def floordiv(self, A, B):
		if isinstance(A, list) != True:
			raise ValueError("add, A is not a list!!")
		if isinstance(B, list) != True:
			raise ValueError("add, B is not a list!!")

		if A == B:
			return [1]
		elif B == [0]:
			raise ValueError("Oops, divide by zero!!")
		elif A == [0]:
			return [0]
		else:
			# Performing division
			answer = A
			index = [1]
			while True:
				answer = self.sub(answer, B)
				if len(answer) <= len(B) and answer < B:
					break
				index = self.add(index, [1])
			return index

	""" Division (mod, remainder) """
	def moddiv(self, A, B):
		if isinstance(A, list) != True:
			raise ValueError("add, A is not a list!!")
		if isinstance(B, list) != True:
			raise ValueError("add, B is not a list!!")

		if A == B:
			return [0]
		elif B == [0]:
			raise ValueError("Oops, divide by zero!!")
		elif A == [0]:
			return [0]
		else:
			# Performing division
			answer = A
			while True:
				answer = self.sub(answer, B)
				if len(answer) <= len(B) and answer < B:
					break

			if answer[0] == 0:
				answer = answer[1:]

			return answer

	"""

	a full adder: just adds A, B, and Carry and returns its result and
	the next Carry

	"""
	def full_adder(self, an, bn, cn):
		rn = an + bn + cn

		if rn >= 10:
			cn_out = rn//10;
			rn = rn%10;
		else:
			cn_out = 0;

		return rn, cn_out

	"""

	full_subtractor: subtracts with A, B and borrow digit and return its result

	"""
	def subtractor(self, an, bn, borrow):
		rn = an - bn - borrow

		if rn < 0:
			rn = rn + 10
			carry_out = 1
		else:
			carry_out = 0

		return rn, carry_out

	"""
	borrow: manipulate digits after borrowing at a certain location

	"""
	def borrow(self, A, position):
		if position < 0:
			position = 0
		elif position > len(A)-1:
			position = len(A) - 1

		if A[i] == 0:
			for i in reversed(list(range(0, position))):
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
		print(self.sign+''.join([str(x) for x in self.lstN]))
	
	def N(self):
		if self.sign == '-':
			self.lstN = [x for x in self.lstN]
		""" Returns lstN into an integer. """
		return int(self.sign+''.join([str(x) for x in self.lstN]))


