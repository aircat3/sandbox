from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QAction, QFileDialog, QGridLayout, QGraphicsView, QGraphicsScene
from PyQt5 import QtGui

import sys
import os

import Canvas

class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        # 追加
        # canvas
        self.canvas = Canvas.Canvas()
        # canvasをMainWindowにセット
        self.setCentralWidget(self.canvas)

        self.setGeometry(300, 300, 500, 500)

        # menubarを作成
        self.createMenubar()

    def createMenubar(self):
        # menubar
        self.menubar = self.menuBar()

        # menubarにメニューを追加
        self.filemenu = self.menubar.addMenu('File')

        # アクションの追加
        self.openAction()

    def openAction(self):
        # アクションの作成
        self.open_act = QAction('開く')
        self.open_act.setShortcut('Ctrl+O') # shortcut
        self.open_act.triggered.connect(self.openFile) # open_actとメソッドを紐づける

        # メニューにアクションを割り当てる
        self.filemenu.addAction(self.open_act)

    def openFile(self):
        self.filepath = QFileDialog.getOpenFileName(self, 'open file', '', 'Images (*.jpeg *.jpg *.png *.bmp)')[0]

        # 追加
        if self.filepath:
            self.canvas.setImage(self.filepath)
            self.resize(self.canvas.pixmap.width(), self.canvas.pixmap.height())
