# -*- coding: utf-8 -*-

from MainWindow import MainWindow
from PyQt4 import QtGui
import sys


ver = '0.0.1.2'

def main():
    app = QtGui.QApplication(sys.argv)
    main = MainWindow(app.clipboard(), ver)
    main.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
