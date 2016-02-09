# -*- coding: utf-8 -*-

from MainWindow import MainWindow
from PyQt4 import QtGui
import os
import sys
import platform as plf

ver = '0.0.2.4'


def main():
    app = QtGui.QApplication(sys.argv)
    args = app.arguments()

    if plf.system() == 'Windows':
        arg_file = args[-1]
    else:
        arg_file = args[1]

    if arg_file and arg_file != os.path.basename(__file__):
        main = MainWindow(app.clipboard(), ver, arg_file)
    else:
        main = MainWindow(app.clipboard(), ver)
    main.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
