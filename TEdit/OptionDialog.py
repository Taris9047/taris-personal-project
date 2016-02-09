# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './OptionWindow.ui'
#
# Created: Mon Jun  1 19:54:25 2015
#      by: PyQt4 UI code generator 4.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui
from Ui_OptionDialog import Ui_OptionsDialog as optUi

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)


class OptionDialog(QtGui.QDialog):
    def __init__(self, parent):
        super(OptionDialog, self).__init__()
        self.parent = parent

        self.Ui = optUi()
        self.Ui.setupUi(self)
