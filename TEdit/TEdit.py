# -*- coding: utf-8 -*-

from MainWindow import MainWindow
from PyQt4 import QtGui
import sys


def main():
    app = QtGui.QApplication(sys.argv)
    main = MainWindow(app.clipboard())
    main.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
