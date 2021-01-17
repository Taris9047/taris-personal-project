#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from MainWindow import MainWindow
from PyQt5.QtWidgets import QApplication
import os
import sys
import platform as plf

ver = '0.3.0'


def main():
    app = QApplication(sys.argv)
    args = app.arguments()

    if len(args) > 1:
        if plf.system() == 'Windows':
            arg_file = args[-1]
        else:
            arg_file = args[1]
    else:
        arg_file = ''

    if arg_file and arg_file != os.path.basename(__file__):
        main = MainWindow(app.clipboard(), ver, arg_file)
    else:
        main = MainWindow(app.clipboard(), ver)
    main.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
