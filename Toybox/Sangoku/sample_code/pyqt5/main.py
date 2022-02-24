from PyQt5.QtWidgets import QApplication, QMainWindow
import sys
import MainWindow

def main():
    # アプリケーションオブジェクトの作成
    app = QApplication(sys.argv)
    # 自分で作ったクラスのインスタンスを生成
    win = MainWindow.MainWindow()
    # ウィジェットの表示
    win.show()
    # アプリケーションの実行
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()