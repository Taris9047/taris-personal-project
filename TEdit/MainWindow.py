# -*- coding: utf-8 -*-
#!/usr/bin/env python

# Form implementation generated from reading ui file './TEditUI.ui'
#
# Created: Sun May 17 15:59:40 2015
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

class MainWindow(QtGui.QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.filename = False
        self.setupUi()

    def setupUi(self):
        self.setObjectName(_fromUtf8("MainWindow"))
        self.resize(640, 700)
        self.centralwidget = QtGui.QWidget(self)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.gridLayout_2 = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout_2.setObjectName(_fromUtf8("gridLayout_2"))
        self.plainTextEdit = QtGui.QPlainTextEdit(self.centralwidget)
        self.plainTextEdit.setObjectName(_fromUtf8("plainTextEdit"))
        self.gridLayout_2.addWidget(self.plainTextEdit, 0, 0, 1, 1)
        self.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(self)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 640, 22))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8("menuFile"))
        self.menuEdit = QtGui.QMenu(self.menubar)
        self.menuEdit.setObjectName(_fromUtf8("menuEdit"))
        self.menuPreferneces = QtGui.QMenu(self.menubar)
        self.menuPreferneces.setObjectName(_fromUtf8("menuPreferneces"))
        self.menuHelp = QtGui.QMenu(self.menubar)
        self.menuHelp.setObjectName(_fromUtf8("menuHelp"))
        self.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(self)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        self.setStatusBar(self.statusbar)
        self.actionOpen = QtGui.QAction(self)
        self.actionOpen.setObjectName(_fromUtf8("actionOpen"))
        self.actionSave = QtGui.QAction(self)
        self.actionSave.setObjectName(_fromUtf8("actionSave"))
        self.actionSave_As = QtGui.QAction(self)
        self.actionSave_As.setObjectName(_fromUtf8("actionSave_As"))
        self.actionCopy = QtGui.QAction(self)
        self.actionCopy.setObjectName(_fromUtf8("actionCopy"))
        self.actionPaste = QtGui.QAction(self)
        self.actionPaste.setObjectName(_fromUtf8("actionPaste"))
        self.actionFind = QtGui.QAction(self)
        self.actionFind.setObjectName(_fromUtf8("actionFind"))
        self.actionFind_and_Replace = QtGui.QAction(self)
        self.actionFind_and_Replace.setObjectName(_fromUtf8("actionFind_and_Replace"))
        self.actionDisplayFonts = QtGui.QAction(self)
        self.actionDisplayFonts.setObjectName(_fromUtf8("actionDisplayFonts"))
        self.actionWord_Wrap = QtGui.QAction(self)
        self.actionWord_Wrap.setObjectName(_fromUtf8("actionWord_Wrap"))
        self.actionIndent_Selection = QtGui.QAction(self)
        self.actionIndent_Selection.setObjectName(_fromUtf8("actionIndent_Selection"))
        self.actionUnindent_Selection = QtGui.QAction(self)
        self.actionUnindent_Selection.setObjectName(_fromUtf8("actionUnindent_Selection"))
        self.actionTabs_to_Space = QtGui.QAction(self)
        self.actionTabs_to_Space.setObjectName(_fromUtf8("actionTabs_to_Space"))
        self.actionOptions = QtGui.QAction(self)
        self.actionOptions.setObjectName(_fromUtf8("actionOptions"))
        self.actionAbout = QtGui.QAction(self)
        self.actionAbout.setObjectName(_fromUtf8("actionAbout"))
        self.actionManual = QtGui.QAction(self)
        self.actionManual.setObjectName(_fromUtf8("actionManual"))
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionSave_As)
        self.menuFile.addSeparator()
        self.menuEdit.addAction(self.actionCopy)
        self.menuEdit.addAction(self.actionPaste)
        self.menuEdit.addAction(self.actionFind)
        self.menuEdit.addAction(self.actionFind_and_Replace)
        self.menuEdit.addSeparator()
        self.menuEdit.addAction(self.actionIndent_Selection)
        self.menuEdit.addAction(self.actionUnindent_Selection)
        self.menuEdit.addAction(self.actionTabs_to_Space)
        self.menuPreferneces.addAction(self.actionDisplayFonts)
        self.menuPreferneces.addAction(self.actionWord_Wrap)
        self.menuPreferneces.addAction(self.actionOptions)
        self.menuHelp.addAction(self.actionAbout)
        self.menuHelp.addAction(self.actionManual)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuEdit.menuAction())
        self.menubar.addAction(self.menuPreferneces.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)

        self.show()

    def retranslateUi(self):
        self.setWindowTitle(_translate("MainWindow", "MainWindow", None))
        self.menuFile.setTitle(_translate("MainWindow", "File", None))
        self.menuEdit.setTitle(_translate("MainWindow", "Edit", None))
        self.menuPreferneces.setTitle(_translate("MainWindow", "Preferneces", None))
        self.menuHelp.setTitle(_translate("MainWindow", "Help", None))
        self.actionOpen.setText(_translate("MainWindow", "&Open", None))
        self.actionSave.setText(_translate("MainWindow", "Save", None))
        self.actionSave_As.setText(_translate("MainWindow", "Save As...", None))
        self.actionCopy.setText(_translate("MainWindow", "Copy", None))
        self.actionPaste.setText(_translate("MainWindow", "Paste", None))
        self.actionFind.setText(_translate("MainWindow", "Find", None))
        self.actionFind_and_Replace.setText(_translate("MainWindow", "Find and Replace", None))
        self.actionDisplayFonts.setText(_translate("MainWindow", "Display Fonts", None))
        self.actionWord_Wrap.setText(_translate("MainWindow", "Word Wrap", None))
        self.actionIndent_Selection.setText(_translate("MainWindow", "Indent Selection", None))
        self.actionUnindent_Selection.setText(_translate("MainWindow", "Unindent Selection", None))
        self.actionTabs_to_Space.setText(_translate("MainWindow", "Tabs to Space", None))
        self.actionOptions.setText(_translate("MainWindow", "Options", None))
        self.actionAbout.setText(_translate("MainWindow", "About", None))
        self.actionManual.setText(_translate("MainWindow", "Manual", None))

