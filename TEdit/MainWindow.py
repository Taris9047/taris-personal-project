# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file './TEditUI.ui'
#
# Created: Sun May 17 15:59:40 2015
#      by: PyQt4 UI code generator 4.11.3
#
# WARNING! All changes made in this file will be lost!

<<<<<<< HEAD
from PyQt5 import QtCore, QtGui
=======
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QAction, QMenuBar, QWidget, QGridLayout, QTextEdit, QStatusBar, QMenu, QFileDialog, QFontDialog
>>>>>>> fd58d09b77a991ad5499339c54f41fda66689d80
from Find import Find, FindnReplace
from OptionDialog import OptionDialog
# from LineTextEdit import LineTextEdit
from SyntaxHighlight import Syntax
from functools import partial
import os
import sys
import ntpath
ntpath.basename("a/b/c")

Debug_Mode = False

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



## MainWindow
#
# Main window of TEdit program.
#
class MainWindow(QtWidgets.QMainWindow):
    file_types = {
        'python': ['.py', '.pyw'],
    }

    ## Constructor
    #
    def __init__(self, clipboard=None, ver='0.0.0.0', arg_file=None):
        super(MainWindow, self).__init__()
        if arg_file:
            self.filename = os.path.abspath(arg_file)
        else:
            self.filename = False

        self.currdir = False
        self.fn = False
        self.curr_path = os.path.dirname(os.path.realpath(__file__))
        self.homedir = self.curr_path
        self.edited = False
        self.tabstop = 4
        self.recent_files = []
        self.recent_files_actions = []
        self.last_file = []
        self.word_wrap = False
        self.Title = 'TEdit - ' + str(ver)
        self.winTitle = self.Title

        self.clipboard = clipboard
        self.setfile_location = './'

        self.setupUi()
        self.setTextWrap()

        self.__setupSettings()
        self.__loadSettings()

    ## __setupSettings
    #
    # Establishes setting management system using QSettings
    #
    def __setupSettings(self):
        if sys.platform == 'darwin':
            self.homedir = os.path.expanduser("~")
            self.setfile_location = self.homedir + '/.config/'

        self.settings = QtCore.QSettings(
            self.setfile_location + 'TEdit.conf',
            QtCore.QSettings.IniFormat)

    ## __saveSettings
    #
    # Saves QSettings variables.
    def __saveSettings(self):
        for i, r in enumerate(self.recent_files):
            self.recent_files[i] = str(r)

        self.recent_files = list(self.recent_files)

        joined_rf = ';'.join(self.recent_files)
        if joined_rf:
            self.settings.setValue('recent files', str(joined_rf))
        if self.font:
            self.settings.setValue('textFont', str(self.font.toString()))
        if self.filename:
            self.settings.setValue('lastFile', str(self.filename))

        self.settings.setValue('Option_WordWrap', self.word_wrap)

    ## __loadSettings
    #
    # reads in settings
    #
    def __loadSettings(self):
        # Read in recent files
        rf_loaded = str(self.settings.value('recent files', type=str))
        if rf_loaded:
            recent_files = rf_loaded.split(';')

            # Check if the files are actually there
            self.recent_files = []
            for rf in recent_files:
                rf = str(rf)
                if os.path.isfile(rf):
                    self.recent_files.append(rf)

        # Update GUI actions to open file via recent files menu.
        self.recent_files_actions = \
            [[] for x in range(len(list(self.recent_files)))]
        for i, rf in enumerate(self.recent_files):
            self.recent_files_actions[i] = QAction(rf, self)

        # Read in fonts
        self.font = QtGui.QFont(
            self.settings.value('textFont', type=str))
        if self.font:
            self.text.setFont(self.font)
        else:
            self.font = QtGui.QFont("Courier New")
            self.font.setPointSize(14)
            self.font.setStyleHint(QtGui.QFont.Monospace)
            self.text.setFont(self.font)

        # reading last file
        self.lastfile = self.settings.value('lastFile', type=str)
        if self.lastfile and self.lastfile != 'False':
            self.__openfile(self.lastfile)

        self.word_wrap = self.settings.value('Option_WordWrap', type=bool)
        self.setTextWrap()

        # refresh main menu to add actions into File menu.
        self.refresh_main_menu()


    ## setupUi
    #
    # establish UI for the editor
    #
    def setupUi(self):
        self.resize(640, 640)

        self.setupActions()

        self.menubar = QMenuBar()
        self.centralwidget = QWidget(self)
        self.gridLayout = QGridLayout(self.centralwidget)
        self.text = QTextEdit(self.centralwidget)
        # self.editor = LineTextEdit(self.centralwidget)
        # self.text = self.editor.edit
        self.gridLayout.addWidget(self.text, 0, 0, 1, 1)
        self.setCentralWidget(self.centralwidget)
        self.text.document().modificationChanged.connect(self.TextEdited)
        self.textCursor = self.text.textCursor()
        self.text.cursorPositionChanged.connect(self.textCursorPosition)

        self.statusbar = QStatusBar(self)
        self.setStatusBar(self.statusbar)

        self.setWindowTitle(self.winTitle)

        self.refresh_main_menu()

    ## setupActions
    #
    # establishes actions for TEdit MainWindow
    #
    def setupActions(self):
        # Actions
        self.actionNew = QAction(r'&New', self)
        self.actionNew.setShortcut('Ctrl+N')
        self.actionNew.triggered.connect(self.NewFile)

        self.actionOpen = QAction(r'&Open', self)
        self.actionOpen.setShortcut('Ctrl+O')
        self.actionOpen.triggered.connect(self.OpenFile)

        self.actionSave = QAction(r'&Save', self)
        self.actionSave.setShortcut('Ctrl+S')
        self.actionSave.triggered.connect(self.SaveFile)

        self.actionSave_As = QAction(r'Save &As...', self)
        self.actionSave_As.setShortcut('Ctrl+Shift+S')
        self.actionSave_As.triggered.connect(self.SaveAs)

        self.actionQuit = QAction(r'&Quit', self)
        self.actionQuit.setShortcut('Ctrl+Q')
        self.actionQuit.triggered.connect(self.close)

        self.actionCopy = QAction(r'&Copy', self)
        self.actionCopy.setShortcut('Ctrl+C')
        self.actionCopy.triggered.connect(self.copyText)

        self.actionPaste = QAction(r'&Paste', self)
        self.actionPaste.setShortcut('Ctrl+P')
        self.actionPaste.triggered.connect(self.pasteText)

        self.actionFind = QAction(r'&Find', self)
        self.actionFind.setShortcut('Ctrl+F')
        self.actionFind.triggered.connect(self.Find_)

        self.actionFind_and_Replace = QAction(r'Find and &Replace', self)
        self.actionFind_and_Replace.setShortcut('Ctrl+Shift+F')
        self.actionFind_and_Replace.triggered.connect(self.Find_and_Replace)

        self.actionDisplayFonts = QAction(r'Fon&ts', self)
        self.actionDisplayFonts.setShortcut('Ctrl+F5')
        self.actionDisplayFonts.triggered.connect(self.setFont)

        self.actionWord_Wrap = QAction(r'Toggle Word Wrap', self)
        self.actionWord_Wrap.setShortcut('Ctrl+Alt+W')
        self.actionWord_Wrap.triggered.connect(self.toggleTextWrap)

        self.actionIndent_Selection = QAction(r'&Indent', self)
        self.actionUnindent_Selection = QAction(r'&Unindent', self)

        self.actionTabs_to_Space = QAction(r'&Tabs to Space', self)
        self.actionTabs_to_Space.setShortcut('Ctrl+Alt+T')
        self.actionTabs_to_Space.triggered.connect(self.TabsToSpace)

        self.actionOptions = QAction(r'&Options', self)
        self.actionOptions.setShortcut('F4')
        self.actionOptions.triggered.connect(self.openOptionDialog)

        self.actionAbout = QAction(r'Abo&ut', self)

        self.actionManual = QAction(r'&Manual', self)
        self.actionManual.setShortcut('F1')


    # refresh_main_menu
    #
    # refreshes main menubar to update recent file.
    #
    def refresh_main_menu(self):
        # Menubar definition
        self.menubar.clear()
        self.menuFile = QMenu(r'&File', self.menubar)
        self.menuEdit = QMenu(r'&Edit', self.menubar)
        self.menuPreferneces = QMenu(r'&Preferences', self.menubar)
        self.menuHelp = QMenu(r'&Help', self.menubar)

        # File Menu
        self.menuFile.addAction(self.actionNew)
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionSave_As)
        self.menuFile.addSeparator()

        if len(self.recent_files_actions) > 0:
            for i, rfa in enumerate(self.recent_files_actions):
                self.recent_files_actions[i].triggered.connect(
                    partial(self.__openfile, filename=self.recent_files[i]))
                self.menuFile.addAction(self.recent_files_actions[i])
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
        self.menuPreferneces.addAction(self.actionOptions)
        self.menuPreferneces.addAction(self.actionWord_Wrap)

        # Help Menu
        self.menuHelp.addAction(self.actionAbout)
        self.menuHelp.addAction(self.actionManual)

        # Generating Main Menu
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuEdit.menuAction())
        self.menubar.addAction(self.menuPreferneces.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.setMenuBar(self.menubar)

    ## __updates_recent_files
    #
    # Update recent file in the main menu.
    #
    def __update_recent_files(self):
        repeated = False
        for rf in self.recent_files:
            if self.filename == rf:
                repeated = True

        if repeated:
            return False
        else:
            self.recent_files.append(str(self.filename))
            self.recent_files_actions.\
                append(QAction(self.filename, self))
            return True

    ## __openfile
    #
    # reads in file contents and sets up parameters for current window
    #
    def __openfile(self, filename):
        if not os.path.exists(filename):
            return None

        with open(filename, 'r') as fp:
            self.text.setPlainText(
                fp.read().replace('\t', ' ' * self.tabstop))
            self.filename = os.path.normpath(filename)
            self.currdir, self.fn = ntpath.split(self.filename)
            self.edited = False
            self.__update_recent_files()
            self.__updateWinTitle()
            self.refresh_main_menu()

            # Highlight syntax if possible.
            syntaxHL = Syntax(self.__filetype(self.filename), self.text)

        return filename

    ## __filetype
    #
    # returns filetype from file extension
    #
    def __filetype(self, filename):
        file_ext = os.path.splitext(filename)[-1]
        for ft in self.file_types.keys():
            if file_ext in self.file_types[ft]:
                return ft
        return 'default'

    ## __savefile
    #
    # Saves file and sets up stuff
    #
    def __savefile(self, filename):
        with open(filename, 'w') as fp:
            fp.write(self.text.toPlainText())
            self.filename = str(filename)
            self.currdir, self.fn = ntpath.split(self.filename)
            self.edited = False
            self.__updateWinTitle()

    ## __updateWinTitle
    #
    # Update window title
    #
    def __updateWinTitle(self):
        if not self.filename:
            filename = 'no file'
        else:
            filename = self.filename
        if self.edited:
            self.winTitle = self.Title + ' - *' + filename
        else:
            self.winTitle = self.Title + ' - ' + filename

        self.setWindowTitle(self.winTitle)

    ## NewFile
    #
    # New File method
    #
    def NewFile(self):
        if self.UnSaved() != True:
            self.text.clear()
            self.filename = False

    ## OpenFile
    #
    # Open file method
    #
    def OpenFile(self):
        if self.UnSaved() != True:
            filename = QFileDialog.\
                getOpenFileName(self, 'Open File',
                    self.homedir if not self.currdir else self.currdir)[0]
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
        if not self.filename:
            filename = QFileDialog.\
                getSaveFileName(self, 'Save File', self.homedir)[0]
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
        filename = QFileDialog.\
            getSaveFileName(self, 'Save File',
                self.homedir if not self.currdir else self.currdir)[0]
        if filename:
            self.__savefile(filename)
            return filename
        else:
            return False

    ## Find_
    #
    # call out Find
    #
    def Find_(self):
        find_ = Find(self)
        find_.show()

    ## Find_and_Replace
    #
    # call out find and replace
    #
    def Find_and_Replace(self):
        find_and_replace = FindnReplace(self)
        find_and_replace.show()

    ## copyText
    #
    # copy text into clipboard
    #
    def copyText(self):
        textSelected = self.textCursor.selectedText()
        self.clipboard.setText(textSelected)

    ## pasteText
    #
    # paste text from clipboard
    #
    def pasteText(self):
        textToPaste = self.clipboard.text
        self.textCursor.insertText(textToPaste)

    ## setFont
    #
    # Opens up font dialog
    #
    def setFont(self):
        self.font, status = QFontDialog.getFont()
        if status:
            self.text.setFont(self.font)
            if Debug_Mode:
                print('Font has set to: ', str(self.font.toString()))

    ## open Options
    #
    # Opens up options dialog
    #
    def openOptionDialog(self):
        self.optionDialog = OptionDialog(self)
        self.optionDialog.show()

    ## UnSaved
    #
    # Detects the un-saved states and pops up dialog
    #
    def UnSaved(self):
        if self.edited:
            answer = QtGui.QMessageBox.question(
                self, "Unsaved changes detected!!",
                "Would you save unsaved changes now?",
                QtGui.QMessageBox.Yes | QtGui.QMessageBox.No |
                QtGui.QMessageBox.Cancel)

            if answer == QtGui.QMessageBox.Cancel:
                return True
            elif answer == QtGui.QMessageBox.No:
                return False
            elif answer == QtGui.QMessageBox.Yes:
                return self.SaveFile()

    ## closeEvent
    #
    # Deals what happens when closing the program.
    #
    def closeEvent(self, e):
        if self.edited:
            if self.UnSaved() == True:
                e.ignore()
            else:
                self.__saveSettings()
                e.accept()
        else:
            self.__saveSettings()
            e.accept()

    ## TextEdited
    #
    # Update window title when the textedit was edited.
    #
    def TextEdited(self):
        self.edited = True
        self.__updateWinTitle()

    ## setTextWrap
    #
    # Set wrap mode for textedit
    #
    def setTextWrap(self):
        if self.word_wrap:
            self.text.setWordWrapMode(
                QtGui.QTextOption.WordWrap)
        else:
            self.text.setWordWrapMode(
                QtGui.QTextOption.NoWrap)

        self.refresh_main_menu()

    ## toggleTextWrap
    #
    # Toggles Word Wrap
    #
    def toggleTextWrap(self):
        if self.word_wrap:
            self.word_wrap = False
        else:
            self.word_wrap = True
        self.setTextWrap()

    ## TabsToSpace
    #
    # Change Tabs to space in the main text form
    #
    def TabsToSpace(self):
        curr_text = self.text.toPlainText()
        curr_text = curr_text.replace('\t', ' ' * self.tabstop)
        self.text.setPlainText(curr_text)

    ## textCursorPosition
    #
    # display current position at statusbar
    #
    def textCursorPosition(self):
        self.textcursor = self.text.textCursor()

        line = self.textcursor.blockNumber() + 1
        col = self.textcursor.columnNumber()

        if Debug_Mode:
            print("Cursor Location: ", line, col)

        self.statusbar.showMessage(
            "Cursor@(line {}, column {})".format(line, col))
