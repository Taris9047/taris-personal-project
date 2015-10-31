# -*- coding: utf-8 -*-
# adopted from
# https://wiki.python.org/moin/PyQt/Python%20syntax%20highlighting

import os, sys
import platform
from PyQt4.QtCore import QRegExp
from PyQt4.QtGui import QColor, QTextCharFormat, QFont, QSyntaxHighlighter
import Styles as sty


## Syntax Selector
#
def Syntax(filetype='text', document=None):
    file_type_map = {
        ".py": 'python',
        ".pyw": 'python',
        "python": 'python',

        "default": 'default',
    }

    if file_type_map[filetype] == 'python':
        return PythonSyntax(document)
    else:
        return None

## PythonSyntax
#
# Syntax highlighter for python
#
class PythonSyntax(QSyntaxHighlighter):
    # Python Keywords
    keywords = [
        'and', 'assert', 'break', 'class', 'continue', 'def',
        'del', 'elif', 'else', 'except', 'exec', 'finally',
        'for', 'from', 'global', 'if', 'import', 'in',
        'is', 'lambda', 'not', 'or', 'pass', 'print',
        'raise', 'return', 'try', 'while', 'yield',
        'None', 'True', 'False', 
    ]

    # Python operators
    operators = [
        '=', '==', '!=', '<', '>', '<=', '=>', '>=', '\+',
        '-', '\*', '/', '//', '\%', '\*\*', '\+=', '-=', '\*=',
        '/=', '\%=', '\^', '\|', '\&', '\~', '>>', '<<',
    ]

    # Python braces
    braces = [
        '\{', '\}', '\(', '\)', '\[', '\]',
    ]

    ## Initializer
    #
    def __init__(self, document, style='default'):
        QSyntaxHighlighter.__init__(self, document)
        Style = sty.Styles(style).getStyle()

        # multi-line comments, a bit buggy 
        self.tri_single = (QRegExp("'''"), 1, Style)
        self.tri_double = (QRegExp('"""'), 2, Style)

        rules = []

        # keywords, operator, brace rules
        rules += [(r'\b%s\b'% w, 0, Style['keyword'])
            for w in PythonSyntax.keywords]
        rules += [(r'%s' % o, 0, Style['operator'])
            for o in PythonSyntax.operators]
        rules += [(r'%s' % b, 0, Style['brace'])
            for b in PythonSyntax.braces]

        # All other rules
        rules += [
            (r'\bself\b', 0, Style['self']),
            (r'"[^"\\]*(\\.[^"\\]*)*"', 0, Style['string']),
            (r"'[^'\\]*(\\.[^'\\]*)*'", 0, Style['string']),
            (r'\bdef\b\s*(\w+)', 1, Style['defclass']),
            (r'\bclass\b\s*(\w+)', 1, Style['defclass']),
            (r'#[^\n]*', 0, Style['comment']),
            (r'\b[+-]?[0-9]+[lL]?\b', 0, Style['numbers']),
            (r'\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\b', 0, Style['numbers']),
            (r'\b[+-]?[0-9]+(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\b', 0, Style['numbers']),
        ]

        self.rules = [(QRegExp(pat), index, fmt) \
            for (pat, index, fmt) in rules]

    ## highlightBlock
    #
    # applies the syntax highlighting given text block
    #
    def highlightBlock(self, text):
        for expression, nth, format in self.rules:
            index = expression.indexIn(text, 0)
            #print(type(expression.cap(nth)))
            while index >= 0:
                index = expression.pos(nth)
                length = len(expression.cap(nth))
                self.setFormat(index, length, format)
                index = expression.indexIn(text, index+length)

        self.setCurrentBlockState(0)

    #     in_multiline = self.match_multiline(text, *self.tri_single)
    #     if not in_multiline:
    #         in_multiline = self.match_multiline(text, *self.tri_double)

    # ## match_multiline
    # #
    # # Highlighting multi-line strings.
    # #
    # def match_multiline(self, text, delimiter, in_state, mlstyle):
    #     if self.previousBlockState() == in_state:
    #         start = 0
    #         add = 0
    #     else:
    #         start = delimiter.indexIn(text)
    #         add = delimiter.matchedLength()

    #     while start >= 0:
    #         end = delimiter.indexIn(text, start+add)

    #         if end >= add:
    #             length = end - start + add + delimiter.matchedLength()
    #             self.setCurrentBlockState(0)
    #         else:
    #             self.setCurrentBlockState(in_state)
    #             length = len(text) - start + add

    #         self.setFormat(start, length, mlstyle)
    #         start = delimiter.indexIn(text, start+length)

    #     if self.setCurrentBlockState() == in_state:
    #         return True
    #     else:
    #         return False
