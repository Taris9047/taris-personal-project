# -*- coding: utf-8 -*-
#!/usr/bin/env python

from MainWindow import *
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import Qt, QThread
import sys

def main():
	app = QtGui.QApplication(sys.argv)
	#app.setStyle('plastique')
	main = MainWindow()
	main.show()
	sys.exit(app.exec_())

if __name__ == '__main__':
	main()