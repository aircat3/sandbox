from PyQt5.QtWidgets import QWidget, QGridLayout, QGraphicsView, QGraphicsScene
from PyQt5 import QtGui

class Canvas(QWidget):
    def __init__(self):
        super(Canvas, self).__init__()

        # canvasのレイアウト
        self.canvas_layout = QGridLayout()
        
        # canvas_layoutをQWidget(self)にセット
        self.setLayout(self.canvas_layout)

        # 画像を表示するためのviewをレイアウトにセット
        self.view = QGraphicsView()
        self.scene = QGraphicsScene()
        self.view.setScene(self.scene)  
        self.canvas_layout.addWidget(self.view)

    def setImage(self, filepath):
        img = QtGui.QImage()
        
        # 画像ファイルの読み込み
        if not img.load(filepath):
            return False

        # sceneの初期化
        self.scene.clear()

        # QImage -> QPixmap
        self.pixmap = QtGui.QPixmap.fromImage(img)

        # pixmapをsceneに追加
        self.scene.addPixmap(self.pixmap)

        # ウィジェットを更新
        self.update()

        return True