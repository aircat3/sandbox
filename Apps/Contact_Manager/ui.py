import sys
from PyQt5.QtWidgets import QMainWindow, QApplication, QCalendarWidget, QWidget, QAction, QTableWidget,QTableWidgetItem,QVBoxLayout
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot

class App(QWidget):

    def __init__(self):
        super().__init__()
        self.title = 'Contact Manager'
        self.left = 0
        self.top = 0
        self.width = 800
        self.height = 600
        self.initUI()

    def initUI(self):
        self.setWindowTitle(self.title)
        self.setGeometry(self.left, self.top, self.width, self.height)

        self.createTable()

        # Add box layout, add table to box layout and add box layout to widget
        self.layout = QVBoxLayout()
        self.cal = QCalendarWidget(self)
        self.cal.setGridVisible(True)
        self.layout.addWidget(self.cal)
        self.layout.addWidget(self.tableWidget)
        self.setLayout(self.layout) 

        # Show widget
        self.show()
    
    def table_appender(self, *args):
        widget = self.tableWidget
        def set_columns(len, pos):
            if pos == len-1:
                widget.setItem(widget.rowCount()-1, pos, QTableWidgetItem(args[pos]))
            else:
                widget.setItem(widget.rowCount()-1, pos, QTableWidgetItem(args[pos]))
                set_columns(len, pos+1)
        widget.insertRow(widget.rowCount())
        set_columns(widget.columnCount(), 0)

    def createTable(self):
        # Create table
        self.tableWidget = QTableWidget()
        self.tableWidget.setRowCount(0)
        self.tableWidget.setColumnCount(6)

        self.tableWidget.setHorizontalHeaderLabels(
            ['名前', '連絡先情報', 'メモ', 'カテゴリ', '最終連絡日', '次回連絡日'])

        self.table_appender('いのり', '@Inori_Nui80', 'Twitterの闇ぬいの人', 'A', '2022/3/8', '2022/4/8')
        self.table_appender('いのり', '@Inori_Nui80', 'Twitterの闇ぬいの人', 'A', '2022/3/8', '2022/4/8')
        
        self.tableWidget.move(0,0)

        # table selection change
        self.tableWidget.doubleClicked.connect(self.on_click)
    
    @pyqtSlot()
    def on_click(self):
        print('\n')
        for currentQTableWidgetItem in self.tableWidget.selectedItems():
            print(currentQTableWidgetItem.row(), currentQTableWidgetItem.column(), currentQTableWidgetItem.text())

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = App()
    sys.exit(app.exec_())