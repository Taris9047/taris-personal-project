# -*- coding: utf-8 -*-

<<<<<<< HEAD
from PyQt5 import QtGui
=======
from PyQt5 import QtGui, QtWidgets
>>>>>>> fd58d09b77a991ad5499339c54f41fda66689d80

import re


class Find(QtWidgets.QDialog):
    # Constructor
    #

    def __init__(self, parent=None):
        QtWidgets.QDialog.__init__(self, parent)
        self.parent = parent
        self.lastMatch = None
        self.Ui()

    def Ui(self):
        self.findButton = QtGui.QPushButton("Find", self)
        self.findButton.clicked.connect(self.find)

        self.findField = QtGui.QTextEdit(self)
        self.findField.resize(250, 15)

        layout = QtGui.QGridLayout()

        layout.addWidget(self.findField, 1, 0, 1, 4)
        layout.addWidget(self.findButton, 2, 0, 1, 2)

        self.setGeometry(300, 300, 360, 50)
        self.setWindowTitle("Find")
        self.setLayout(layout)

    def find(self):
        # Grab parent's text
        text = self.parent.text.toPlainText()

        # And the text to find as well
        query = self.findField.toPlainText()
        flags = re.I

        # Compile the pattern
        pattern = re.compile(query, flags)

        # Continue to next match
        start = self.lastMatch.start() + 1 if self.lastMatch else 0

        # The actual search
        self.lastMatch = pattern.search(text, start)

        if self.lastMatch:
            start = self.lastMatch.start()
            end = self.lastMatch.end()

            self.moveCursor(start, end)

        else:
            self.parent.text.moveCursor(QtGui.QTextCursor.End)

    def moveCursor(self, start, end):

        # We retrieve the QTextCursor object from the parent's QTextEdit
        cursor = self.parent.text.textCursor()

        # Then we set the position to the beginning of the last match
        cursor.setPosition(start)

        # Next we move the Cursor by over the match and pass the KeepAnchor parameter
        # which will make the cursor select the the match's text
        cursor.movePosition(
            QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor, end - start)

        # And finally we set this new cursor as the parent's
        self.parent.text.setTextCursor(cursor)


class FindnReplace(QtWidgets.QDialog):
    # Constructor
    #

    def __init__(self, parent=None):
        QWidget.QDialog.__init__(self, parent)
        self.parent = parent
        self.lastMatch = None
        self.Ui()

    def Ui(self):

        # Button to search the document
        self.findButton = QtGui.QPushButton("Find", self)
        self.findButton.clicked.connect(self.find)

        # Button to replace the last finding
        self.replaceButton = QtGui.QPushButton("Replace", self)
        self.replaceButton.clicked.connect(self.replace)

        # Button to replace all findings
        self.repAllButton = QtGui.QPushButton("Replace All", self)
        self.repAllButton.clicked.connect(self.replaceAll)

        # Normal Mode radio button
        self.normalRadio = QtGui.QRadioButton("Normal", self)
        self.normalRadio.toggled.connect(self.normalMode)

        # Regular Expression Mode radio button
        self.regexRadio = QtGui.QRadioButton("RegEx", self)
        self.regexRadio.toggled.connect(self.regexMode)

        # Find text field
        self.findField = QtGui.QTextEdit(self)
        self.findField.resize(250, 20)

        # Replace text field
        self.replaceField = QtGui.QTextEdit(self)
        self.replaceField.resize(250, 20)

        # Options Label
        self.optionsLabel = QtGui.QLabel("Options: ", self)

        # Case sensitivity
        self.caseSens = QtGui.QCheckBox("Case sensitivie", self)

        # Whole words option
        self.wholeWords = QtGui.QCheckBox("Whole Words", self)

        # Layout objects
        self.layout = QtGui.QGridLayout()

        self.layout.addWidget(self.findField, 1, 0, 1, 4)
        self.layout.addWidget(self.normalRadio, 2, 2)
        self.layout.addWidget(self.regexRadio, 2, 3)
        self.layout.addWidget(self.findButton, 2, 0, 1, 2)
        self.layout.addWidget(self.replaceField, 3, 0, 1, 4)
        self.layout.addWidget(self.replaceButton, 4, 0, 1, 2)
        self.layout.addWidget(self.repAllButton, 4, 2, 1, 2)

        # Add some spacing
        self.spacer = QtGui.QWidget(self)
        self.spacer.setFixedSize(0, 10)
        self.layout.addWidget(self.spacer, 5, 0)
        self.layout.addWidget(self.optionsLabel, 6, 0)
        self.layout.addWidget(self.caseSens, 6, 1)
        self.layout.addWidget(self.wholeWords, 6, 2)

        self.setGeometry(0, 0, 360, 150)
        self.setWindowTitle("Find and Replace")
        self.setLayout(self.layout)

        # Normal mode is activated by default
        self.normalRadio.setChecked(True)

    def find(self):
        # Grab parent's text
        text = self.parent.text.toPlainText()

        # And the text to find as well
        query = self.findField.toPlainText()

        # Whole Words mode
        if self.wholeWords.isChecked():
            query = r'\W' + query + r'\W'

        # detect if regEx is on
        flags = 0 if self.caseSens.isChecked() else re.I

        # Compile the pattern
        pattern = re.compile(query, flags)

        # Continue to next match
        start = self.lastMatch.start() + 1 if self.lastMatch else 0

        # The actual search
        self.lastMatch = pattern.search(text, start)

        if self.lastMatch:
            start = self.lastMatch.start()
            end = self.lastMatch.end()

            if self.wholeWords.isChecked():
                start += 1
                end -= 1

            self.moveCursor(start, end)

        else:
            self.parent.text.moveCursor(QtGui.QTextCursor.End)

    def replace(self):

        # Grab the text cursor
        cursor = self.parent.text.textCursor()

        # Security
        if self.lastMatch and cursor.hasSelection():

            # We insert the new text, which will override the selected
            # text
            cursor.insertText(self.replaceField.toPlainText())

            # And set the new cursor
            self.parent.text.setTextCursor(cursor)

    def replaceAll(self):

        # Set lastMatch to None so that the search
        # starts from the beginning of the document
        self.lastMatch = None

        # Initial find() call so that lastMatch is
        # potentially not None anymore
        self.find()

        # Replace and find until find is None again
        while self.lastMatch:
            self.replace()
            self.find()

    def regexMode(self):

        # First uncheck the checkboxes
        self.caseSens.setChecked(False)
        self.wholeWords.setChecked(False)

        # Then disable them (gray them out)
        self.caseSens.setEnabled(False)
        self.wholeWords.setEnabled(False)

    def normalMode(self):

        # Enable checkboxes (un-gray them)
        self.caseSens.setEnabled(True)
        self.wholeWords.setEnabled(True)

    def moveCursor(self, start, end):

        # We retrieve the QTextCursor object from the parent's QTextEdit
        cursor = self.parent.text.textCursor()

        # Then we set the position to the beginning of the last match
        cursor.setPosition(start)

        # Next we move the Cursor by over the match and pass the KeepAnchor parameter
        # which will make the cursor select the the match's text
        cursor.movePosition(
            QtGui.QTextCursor.Right, QtGui.QTextCursor.KeepAnchor, end - start)

        # And finally we set this new cursor as the parent's
        self.parent.text.setTextCursor(cursor)
