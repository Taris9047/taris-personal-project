#!/usr/bin/env python
from PyQt4.QtGui import *
from ui_SimulationOutput import Ui_SimulationOutput

def main():
    app = QApplication(sys.argv)
    window = QDialog()
    ui = Ui_SimulationOutput()
    ui.setupUi(window)

    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
