# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '.\SimulationOutput.ui'
#
# Created: Mon May  4 19:04:04 2015
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

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(559, 399)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.outputText = QtGui.QTextBrowser(self.centralwidget)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.outputText.sizePolicy().hasHeightForWidth())
        self.outputText.setSizePolicy(sizePolicy)
        self.outputText.setSizeIncrement(QtCore.QSize(0, 0))
        font = QtGui.QFont()
        font.setFamily(_fromUtf8("Bitstream Vera Sans Mono"))
        font.setPointSize(10)
        self.outputText.setFont(font)
        self.outputText.setAutoFillBackground(False)
        self.outputText.setLocale(QtCore.QLocale(QtCore.QLocale.English, QtCore.QLocale.UnitedStates))
        self.outputText.setAcceptRichText(False)
        self.outputText.setObjectName(_fromUtf8("outputText"))
        self.gridLayout.addWidget(self.outputText, 0, 0, 1, 1)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 559, 21))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuSave_Log = QtGui.QMenu(self.menubar)
        self.menuSave_Log.setObjectName(_fromUtf8("menuSave_Log"))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        MainWindow.setStatusBar(self.statusbar)
        self.actionSave_Log = QtGui.QAction(MainWindow)
        self.actionSave_Log.setObjectName(_fromUtf8("actionSave_Log"))
        self.actionQuit = QtGui.QAction(MainWindow)
        self.actionQuit.setObjectName(_fromUtf8("actionQuit"))
        self.menuSave_Log.addAction(self.actionSave_Log)
        self.menuSave_Log.addSeparator()
        self.menuSave_Log.addAction(self.actionQuit)
        self.menubar.addAction(self.menuSave_Log.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "SimulationOutput", None))
        self.menuSave_Log.setTitle(_translate("MainWindow", "&Log", None))
        self.actionSave_Log.setText(_translate("MainWindow", "Save Log", None))
        self.actionQuit.setText(_translate("MainWindow", "Quit", None))

