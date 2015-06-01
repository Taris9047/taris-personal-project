# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './OptionWindow.ui'
#
# Created: Mon Jun  1 19:54:25 2015
#      by: PyQt4 UI code generator 4.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

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

        self.setupUi()
        self.init_menus()


    def setupUi(self):
        self.setObjectName(_fromUtf8("OptionsDialog"))
        self.resize(300, 240)
        self.setMinimumSize(QtCore.QSize(300, 240))
        self.setMaximumSize(QtCore.QSize(300, 240))
        self.gridLayout = QtGui.QGridLayout(self)
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.LBGroupBox = QtGui.QGroupBox(self)
        self.LBGroupBox.setObjectName(_fromUtf8("LBGroupBox"))
        self.LBMacRadioButton = QtGui.QRadioButton(self.LBGroupBox)
        self.LBMacRadioButton.setGeometry(QtCore.QRect(10, 21, 111, 18))
        self.LBMacRadioButton.setObjectName(_fromUtf8("LBMacRadioButton"))
        self.LBLinuxRadioButton = QtGui.QRadioButton(self.LBGroupBox)
        self.LBLinuxRadioButton.setGeometry(QtCore.QRect(140, 20, 81, 18))
        self.LBLinuxRadioButton.setObjectName(_fromUtf8("LBLinuxRadioButton"))
        self.LBDOSRadioButton = QtGui.QRadioButton(self.LBGroupBox)
        self.LBDOSRadioButton.setGeometry(QtCore.QRect(10, 39, 151, 18))
        self.LBDOSRadioButton.setObjectName(_fromUtf8("LBDOSRadioButton"))
        self.gridLayout.addWidget(self.LBGroupBox, 2, 0, 1, 1)
        self.wordWrapRadioButton = QtGui.QRadioButton(self)
        self.wordWrapRadioButton.setObjectName(_fromUtf8("wordWrapRadioButton"))
        self.gridLayout.addWidget(self.wordWrapRadioButton, 1, 0, 1, 1)
        self.buttonBox = QtGui.QDialogButtonBox(self)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName(_fromUtf8("buttonBox"))
        self.gridLayout.addWidget(self.buttonBox, 4, 0, 1, 1)
        self.showLineNumberRadioButton = QtGui.QRadioButton(self)
        self.showLineNumberRadioButton.setObjectName(_fromUtf8("showLineNumberRadioButton"))
        self.gridLayout.addWidget(self.showLineNumberRadioButton, 0, 0, 1, 1)
        self.TabStopGroupBox = QtGui.QGroupBox(self)
        self.TabStopGroupBox.setObjectName(_fromUtf8("TabStopGroupBox"))
        self.tabstopSpinBox = QtGui.QSpinBox(self.TabStopGroupBox)
        self.tabstopSpinBox.setGeometry(QtCore.QRect(10, 20, 231, 24))
        self.tabstopSpinBox.setProperty("value", 4)
        self.tabstopSpinBox.setObjectName(_fromUtf8("tabstopSpinBox"))
        self.gridLayout.addWidget(self.TabStopGroupBox, 3, 0, 1, 1)

        self.retranslateUi()
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("accepted()")), self.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("rejected()")), self.reject)
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        self.setWindowTitle(_translate("OptionsDialog", "TEdit - Options", None))
        self.LBGroupBox.setTitle(_translate("OptionsDialog", "Linebreak Options", None))
        self.LBMacRadioButton.setText(_translate("OptionsDialog", "Mac OS X (CR)", None))
        self.LBLinuxRadioButton.setText(_translate("OptionsDialog", "Linux (LF)", None))
        self.LBDOSRadioButton.setText(_translate("OptionsDialog", "Windows (CR/LF)", None))
        self.wordWrapRadioButton.setText(_translate("OptionsDialog", "Word Wrap", None))
        self.showLineNumberRadioButton.setText(_translate("OptionsDialog", "Show Line Number", None))
        self.TabStopGroupBox.setTitle(_translate("OptionsDialog", "Tabstop", None))


    def init_menus(self):
        if self.parent.word_wrap:
            self.wordWrapRadioButton.setChecked(True)

        
