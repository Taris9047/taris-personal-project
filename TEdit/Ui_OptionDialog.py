# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './OptionDialog.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
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

class Ui_OptionsDialog(object):
    def setupUi(self, OptionsDialog):
        OptionsDialog.setObjectName(_fromUtf8("OptionsDialog"))
        OptionsDialog.resize(300, 240)
        OptionsDialog.setMinimumSize(QtCore.QSize(300, 240))
        OptionsDialog.setMaximumSize(QtCore.QSize(300, 240))
        self.gridLayout = QtGui.QGridLayout(OptionsDialog)
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.showLineNumberRadioButton = QtGui.QRadioButton(OptionsDialog)
        self.showLineNumberRadioButton.setObjectName(_fromUtf8("showLineNumberRadioButton"))
        self.gridLayout.addWidget(self.showLineNumberRadioButton, 0, 0, 1, 1)
        self.wordWrapRadioButton = QtGui.QRadioButton(OptionsDialog)
        self.wordWrapRadioButton.setObjectName(_fromUtf8("wordWrapRadioButton"))
        self.gridLayout.addWidget(self.wordWrapRadioButton, 1, 0, 1, 1)
        self.LBGroupBox = QtGui.QGroupBox(OptionsDialog)
        self.LBGroupBox.setObjectName(_fromUtf8("LBGroupBox"))
        self.gridLayout_2 = QtGui.QGridLayout(self.LBGroupBox)
        self.gridLayout_2.setObjectName(_fromUtf8("gridLayout_2"))
        self.LBDOSRadioButton = QtGui.QRadioButton(self.LBGroupBox)
        self.LBDOSRadioButton.setObjectName(_fromUtf8("LBDOSRadioButton"))
        self.gridLayout_2.addWidget(self.LBDOSRadioButton, 1, 0, 1, 2)
        self.LBMacRadioButton = QtGui.QRadioButton(self.LBGroupBox)
        self.LBMacRadioButton.setObjectName(_fromUtf8("LBMacRadioButton"))
        self.gridLayout_2.addWidget(self.LBMacRadioButton, 0, 0, 1, 1)
        self.LBLinuxRadioButton = QtGui.QRadioButton(self.LBGroupBox)
        self.LBLinuxRadioButton.setObjectName(_fromUtf8("LBLinuxRadioButton"))
        self.gridLayout_2.addWidget(self.LBLinuxRadioButton, 0, 1, 1, 1)
        self.gridLayout.addWidget(self.LBGroupBox, 2, 0, 1, 1)
        self.TabStopGroupBox = QtGui.QGroupBox(OptionsDialog)
        self.TabStopGroupBox.setObjectName(_fromUtf8("TabStopGroupBox"))
        self.gridLayout_3 = QtGui.QGridLayout(self.TabStopGroupBox)
        self.gridLayout_3.setObjectName(_fromUtf8("gridLayout_3"))
        self.tabstopSpinBox = QtGui.QSpinBox(self.TabStopGroupBox)
        self.tabstopSpinBox.setProperty("value", 4)
        self.tabstopSpinBox.setObjectName(_fromUtf8("tabstopSpinBox"))
        self.gridLayout_3.addWidget(self.tabstopSpinBox, 0, 0, 1, 1)
        self.gridLayout.addWidget(self.TabStopGroupBox, 3, 0, 1, 1)
        self.buttonBox = QtGui.QDialogButtonBox(OptionsDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName(_fromUtf8("buttonBox"))
        self.gridLayout.addWidget(self.buttonBox, 4, 0, 1, 1)

        self.retranslateUi(OptionsDialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("accepted()")), OptionsDialog.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("rejected()")), OptionsDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(OptionsDialog)

    def retranslateUi(self, OptionsDialog):
        OptionsDialog.setWindowTitle(_translate("OptionsDialog", "TEdit - Options", None))
        self.showLineNumberRadioButton.setText(_translate("OptionsDialog", "Show Line Number", None))
        self.wordWrapRadioButton.setText(_translate("OptionsDialog", "Word Wrap", None))
        self.LBGroupBox.setTitle(_translate("OptionsDialog", "Linebreak Options", None))
        self.LBDOSRadioButton.setText(_translate("OptionsDialog", "Windows (CR/LF)", None))
        self.LBMacRadioButton.setText(_translate("OptionsDialog", "Mac OS X (CR)", None))
        self.LBLinuxRadioButton.setText(_translate("OptionsDialog", "Linux (LF)", None))
        self.TabStopGroupBox.setTitle(_translate("OptionsDialog", "Tabstop", None))

