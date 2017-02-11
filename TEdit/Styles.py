# -*- coding: utf-8 -*-

from PyQt4.QtGui import QColor, QTextCharFormat, QFont, QSyntaxHighlighter

class Styles:
    def __init__(self, styname='default'):
        self.STYLES = {
            'default': {
                'keyword': self.format('blue'),
                'operator': self.format('red'),
                'brace': self.format('darkGray'),
                'defclass': self.format('black', 'bold'),
                'string': self.format('magneta'),
                'string2': self.format('darkMagneta'),
                'comment': self.format('darkGreen', 'italic'),
                'self': self.format('black', 'italic'),
                'numbers': self.format('brown')
            }
        }

        self.styname = styname

    ## getStyle
    #
    # returns style dict
    def getStyle(self):
        return self.STYLES[self.styname]

    ## format
    #
    # Return a QTextCharFormat with the given attributes
    #
    def format(self, color, style=''):
        _color = QColor()
        _color.setNamedColor(color)
        _format = QTextCharFormat()
        _format.setForeground(_color)
        if 'bold' in style:
            _format.setFontWeight(QFont.Bold)
        if 'italic' in style:
            _format.setFontItalic(True)

        return _format
