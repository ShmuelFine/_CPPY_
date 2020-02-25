import sys
from typing import Union

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QCursor


class LBL(QtWidgets.QMainWindow):
    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)
        self.setGeometry(1000, 100, 500, 500)
        self.x = -30
        self.LEdit()


    def LEdit(self):
        self.x += 50
        self.LineEdit = QtWidgets.QLineEdit(self)
        self.LineEdit.setStyleSheet("border:none; font-size: 17px; color: gray; ")
        self.LineEdit.setGeometry(50, self.x, 350, 50)
        self.LineEdit.setVisible(True)
        self.inputString = self.LineEdit.text()

        self.arrows = QtWidgets.QLabel(self)
        self.arrows.setGeometry(10, self.x, 40, 50)
        self.arrows.setStyleSheet("border:none; font-size: 17px; color: gray;")
        self.arrows.setText(str(">>>"))
        self.arrows.setVisible(True)
       # self.LineEdit.setCursor()

        self.lbl = QtWidgets.QLabel(self)
        self.lbl.setGeometry(30, self.x, 350, 50)
        self.lbl.setStyleSheet("border:none; font-size: 17px; color: red")
        self.lbl.setVisible(False)

    # def hasNumbers(self):
    #      for char in self.inputString:
    #          if char.isdigit():
    #              char.s
    #     return

    def on_click(self):
        self.lbl.setText(str(self.LineEdit.text()))
        self.lbl.setVisible(True)
        self.LineEdit.setVisible(False)
        self.arrows.setVisible(False)
        self.LEdit()

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Return:
            self.on_click()


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    ui = LBL()
    ui.show()
    sys.exit(app.exec_())
