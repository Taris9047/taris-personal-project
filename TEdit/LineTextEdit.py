# -*- coding: utf-8 -*-

from PyQt4 import QtGui


class LineTextEdit(QtGui.QFrame):

	class NumberBar(QtGui.QWidget):
		def __init__(self, *args):
			QtGui.QWidget.__init__(self, *args)
			self.textedit = None
			self.highest_line = 0

		def setTextEdit(self, textedit):
			self.textedit = textedit

		def update(self, *args):
			width = self.fontMetrics().width(str(self.highest_line))

			if self.width() != width:
				self.setFixedWidth(width)
			QtGui.QWidget.update(self, *args)

		def paintEvent(self, event):
			contents_y = self.textedit.verticalScrollBar().value()
			page_bottom = contents_y + \
				self.textedit.viewport().height()
			font_metrics = self.fontMetrics()
			current_block = \
				self.textedit.document().\
					findBlock(self.textedit.textCursor().position())

			painter = QtGui.QPainter(self)

			line_count = 0

			block = self.textedit.document().begin()
			while block.isValid():
				line_count += 1

				position = \
					self.textedit.document().documentLayout().\
					blockBoundingRect(block).topLeft()

				if position.y() > page_bottom:
					break

				bold = False
				if block == current_block:
					bold = True
					font = painter.font()
					font.setBold(True)
					painter.setFont(font)

				painter.drawText(\
					self.width() - font_metrics.width(str(line_count)) - 3,\
						round(position.y()) - contents_y + \
						font_metrics.ascent(), str(line_count))

				if bold:
					font = painter.font()
					font.setBold(False)
					painter.setFont(font)

				block = block.next()

			self.highest_line = line_count
			painter.end()

			QtGui.QWidget.paintEvent(self, event)


	def __init__(self, *args):
		QtGui.QFrame.__init__(self, *args)

		self.setFrameStyle(\
			QtGui.QFrame.StyledPanel | QtGui.QFrame.Sunken)

		self.textedit = QtGui.QTextEdit()
		self.textedit.setFrameStyle(QtGui.QFrame.NoFrame)
		self.textedit.setAcceptRichText(False)

		self.number_bar = self.NumberBar()
		self.number_bar.setTextEdit(self.textedit)

		hbox = QtGui.QHBoxLayout(self)
		hbox.setSpacing(0)
		hbox.setMargin(0)
		hbox.addWidget(self.number_bar)
		hbox.addWidget(self.textedit)

		self.textedit.installEventFilter(self)
		self.textedit.viewport().installEventFilter(self)

	def eventFilter(self, object, event):
		if object in (self.textedit, self.textedit.viewport()):
			self.number_bar.update()
			return False
		return QtGui.QFrame.eventFilter(object, event)

	def getTextEdit(self):
		return self.textedit






