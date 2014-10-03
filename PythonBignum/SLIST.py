#!/usr/bin/env python
class SList:
	__init__(self, initdata):
		self.content = initdata
		self.nextList = None

	def read_content(self):
		return self.content

	def read_nextList(self):
		return self.nextList

	def set_content(self, data):
		self.content = data

	def set_nextList(self, newnextList):
		self.nextList = newnextList
