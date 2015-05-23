# -*- coding: utf-8 -*-
#!/usr/bin/env python

# Form implementation generated from reading ui file './TEditUI.ui'
#
# Created: Sun May 17 15:59:40 2015
#      by: PyQt4 UI code generator 4.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

version = '0.0.0.2'
Title = 'TEdit '+version

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.\
            translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.\
            translate(context, text, disambig)

class MainWindow(QtGui.QMainWindow):
    ## Constructor
    #
    def __init__(self):
        super(MainWindow, self).__init__()
        self.filename = False
        self.edited = False
        self.tabstop = 4
        self.recent_files = []
        self.recent_files_actions = []
        self.word_wrap = False
        self.winTitle = Title
        self.font = QtGui.QFont("Courier New")
        self.font.setPointSize(14)
        self.font.setStyleHint(QtGui.QFont.Monospace)

        self.setupUi()


    ## setupUi
    #
    # establish UI for the editor
    #
    def setupUi(self):
        self.resize(640, 700)
        self.menubar = QtGui.QMenuBar(self)
        self.centralwidget = QtGui.QWidget(self)
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.plainTextEdit = QtGui.QPlainTextEdit(self.centralwidget)
        self.plainTextEdit.setFont(self.font)
        self.gridLayout.addWidget(self.plainTextEdit, 0, 0, 1, 1)
        self.setCentralWidget(self.centralwidget)
        self.plainTextEdit.document()\
            .modificationChanged.connect(self.TextEdited)

        self.statusbar = QtGui.QStatusBar(self)
        self.setStatusBar(self.statusbar)

        self.refresh_main_menu()

        self.setWindowTitle(self.winTitle)

        self.show()

    ## refresh_main_menu
    #
    # refreshes main menubar to update recent file.
    #
    def refresh_main_menu(self):
        # Menubar definition
        self.menubar.clear()
        self.menuFile = QtGui.QMenu(r'&File', self.menubar)
        self.menuEdit = QtGui.QMenu(r'&Edit',self.menubar)
        self.menuPreferneces = QtGui.QMenu(r'&Preferences',self.menubar)
        self.menuHelp = QtGui.QMenu(r'&Help',self.menubar)
        self.setMenuBar(self.menubar)

        # Actions
        self.actionNew = QtGui.QAction(r'&New',self)
        self.actionNew.setShortcut('Ctrl+N')
        self.actionNew.triggered.connect(self.NewFile)
        self.actionOpen = QtGui.QAction(r'&Open',self)
        self.actionOpen.setShortcut('Ctrl+O')
        self.actionOpen.triggered.connect(self.OpenFile)
        self.actionSave = QtGui.QAction(r'&Save',self)
        self.actionSave.setShortcut('Ctrl+S')
        self.actionSave.triggered.connect(self.SaveFile)
        self.actionSave_As = QtGui.QAction(r'Save &As...',self)
        self.actionSave_As.setShortcut('Ctrl+Shift+S')
        self.actionSave_As.triggered.connect(self.SaveAs)
        self.actionQuit = QtGui.QAction(r'&Quit',self)
        self.actionQuit.setShortcut('Ctrl+Q')
        self.actionQuit.triggered.connect(self.close)

        self.actionCopy = QtGui.QAction(r'&Copy',self)
        self.actionPaste = QtGui.QAction(r'&Paste',self)
        self.actionFind = QtGui.QAction(r'&Find',self)
        self.actionFind_and_Replace = \
            QtGui.QAction(r'Find and &Replace',self)
        self.actionDisplayFonts = QtGui.QAction(r'Fon&ts',self)
        self.actionDisplayFonts.setShortcut('Ctrl+F5')
        self.actionDisplayFonts.triggered.connect(self.setFont)
        self.actionWord_Wrap = QtGui.QAction(r'&Word Wrap',self)
        self.actionIndent_Selection = QtGui.QAction(r'&Indent',self)
        self.actionUnindent_Selection = QtGui.QAction(r'&Unindent',self)
        self.actionTabs_to_Space = QtGui.QAction(r'&Tabs to Space',self)
        
        self.actionOptions = QtGui.QAction(r'&Options',self)
        self.actionAbout = QtGui.QAction(r'Abo&ut',self)
        self.actionManual = QtGui.QAction(r'&Manual',self)

        # File Menu
        self.menuFile.addAction(self.actionNew)
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionSave_As)
        self.menuFile.addSeparator()
        if len(self.recent_files_actions) > 0:
            for i, ra in enumerate(self.recent_files_actions):
                self.menuFile.addAction(ra)
            self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionQuit)

        # Edit Menu
        self.menuEdit.addAction(self.actionCopy)
        self.menuEdit.addAction(self.actionPaste)
        self.menuEdit.addAction(self.actionFind)
        self.menuEdit.addAction(self.actionFind_and_Replace)
        self.menuEdit.addSeparator()
        self.menuEdit.addAction(self.actionIndent_Selection)
        self.menuEdit.addAction(self.actionUnindent_Selection)
        self.menuEdit.addAction(self.actionTabs_to_Space)
        
        # Preference Menu
        self.menuPreferneces.addAction(self.actionDisplayFonts)
        self.menuPreferneces.addAction(self.actionWord_Wrap)
        self.menuPreferneces.addAction(self.actionOptions)
        
        # Help Menu
        self.menuHelp.addAction(self.actionAbout)
        self.menuHelp.addAction(self.actionManual)

        
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuEdit.menuAction())
        self.menubar.addAction(self.menuPreferneces.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())


    ## updates_recent_files
    #
    # Update recent file in the main menu.
    #
    def update_recent_files(self, filename):
        repeated = False
        for rf in self.recent_files:
            if filename == rf:
                repeated = True

        if repeated:
            pass
        else:
            self.recent_files.append(filename)
            self.recent_files_actions.\
                append(QtGui.QAction(filename, self))

    ## __openfile
    #
    # reads in file contents and sets up parameters for current window
    #
    def __openfile(self, filename):
        with open(filename, 'r') as fp:
            self.plainTextEdit.setPlainText(\
                fp.read().replace('\t',' '*self.tabstop))
        self.filename = filename
        self.edited = False
        self.update_recent_files(self.filename)
        self.refresh_main_menu()
        self.__updateWinTitle(self.filename, self.edited)

    ## __savefile
    #
    # Saves file and sets up stuff 
    #
    def __savefile(self, filename):
        with open(filename, 'w') as fp:
            fp.write(self.plainTextEdit.toPlainText())
        self.filename = filename
        self.edited = False
        self.__updateWinTitle(self.filename, self.edited)

    ## __updateWinTitle
    #
    # Update window title
    #
    def __updateWinTitle(self, filename, unsaved=False):
        if filename == False:
            filename = 'no file'
        if unsaved:
            self.winTitle = Title + ' - *' + filename
        else:
            self.winTitle = Title + ' - ' + filename
        #print(self.winTitle)
        self.setWindowTitle(self.winTitle)


    ## NewFile
    #
    # New File method
    #
    def NewFile(self):
        self.plainTextEdit.clear()
        self.filename = False

    ## OpenFile
    #
    # Open file method
    #
    def OpenFile(self):
        filename = QtGui.QFileDialog.\
            getOpenFileName(self, 'Open File', './')
        if filename:
            self.__openfile(filename)
            return filename
        else:
            return False

    ## SaveFile
    #
    # Save file method
    #
    def SaveFile(self):
        if self.filename == False:
            filename = QtGui.QFileDialog.\
                getSaveFileName(self, 'Save File', './')
            if filename:
                self.__savefile(filename)
                return True
            else:
                return False
        else:
            self.__savefile(self.filename)
            return True

    ## SaveAs
    #
    # Save As function
    #
    def SaveAs(self):
        filename = QtGui.QFileDialog.\
            getSaveFileName(self, 'Save File', './')
        if filename:
            self.__savefile(filename)
            return filename
        else:
            return False

    ## setFont
    #
    # Opens up font dialog
    #
    def setFont(self):
        self.font, status = QtGui.QFontDialog.getFont()
        if status:
            self.plainTextEdit.setFont(self.font)


    ## UnSaved
    #
    # Detects the un-saved states and pops up dialog
    #
    def UnSaved(self):
        answer = QtGui.QMessageBox.question(self,
            "Unsaved changes detected!!",
            "Would you save unsaved changes now?",
            QtGui.MessageBox.Yes|QMessageBox.No|
            QMessageBox.Cancel)

        if answer == QMessageBox.Cancel:
            return True
        elif answer == QMessageBox.No:
            return False
        elif answer == QmessageBox.Yes:
            return self.SaveFile()

    ## eventClose
    #
    # Deals what happens when closing the program.
    #
    def eventClose(self, e):
        if self.edited == True:
            self.UnSaved()
            e.ignore()
        else:
            exit()


    ## TextEdited
    #
    # Update window title when the textedit was edited.
    #
    def TextEdited(self):
        self.edited = True
        self.__updateWinTitle(self.filename, self.edited)




        
