from ctypes import alignment
import PySimpleGUI as sg
import tkinter as tk
from PIL import Image, ImageTk

def initMap(canvas):
    line_color = 'black'

    # 1マスのサイズ(px)
    grid_size = 30
    # マスの縦・横数
    grid_width = 9
    grid_height = 9

    # マップの方眼紙の描画(線の数とマス数で2線のズレを修正)
    grid_width = grid_width + 2
    grid_height = grid_height + 2
    for x_pos in range(grid_width):
        for y_pos in range(grid_height):
            # ヨコ線描画
            canvas.tk_canvas.create_line(
                grid_size,
                y_pos * grid_size,
                (grid_width-1) * grid_size,
                y_pos * grid_size
            )
            # タテ線描画
            canvas.tk_canvas.create_line(
                x_pos * grid_size,
                grid_size,
                x_pos * grid_size,
                (grid_height-1) * grid_size
            )

def initGUI():

    # Windowテーマの設定
    #sg.theme_previewer()
    #sg.theme('dark grey 9')
    sg.theme('SystemDefault')
    
    # Windowレイアウトの設定
    col1 = [
        [sg.Text('戦闘状況')],
        [sg.Multiline(
            default_text="hello\nworld\n!!", 
            font=('Arial',14),
            size=(40,12),
            disabled=True)
        ]
    ]
    col2 = [[sg.Frame('部隊状況',[
        [sg.Text('部隊１')],
        [sg.Text('部隊２', size=(50,15))],
    ])]]
    col3 = [
        [sg.Text('コマンド')],
        [sg.InputText('', font=('Arial',14), size=(100, 30))]
    ]
    layout = [
        [sg.Column(col1), sg.Column(col2)],
        [sg.Column(col3)]
    ]
    '''
    canvas = sg.Canvas(size=(320,350))
    layout = [
        [sg.Text('ダウンロードするURLを入力してね！')],
        [sg.Text('ダウンロード先URL', size=(10, 1)), sg.InputText()],
        [sg.Text('保存場所', size=(10, 1)), sg.InputText()],   
        [sg.Button('OK'), sg.Button('キャンセル')],
        [canvas]
    ]
    '''
    

    # Windowの生成
    window = sg.Window('サンプルプログラム', layout, size=(500, 300), margins=(0,0))
    
    # イベント処理
    while True:
        event, values = window.read()
        if event == sg.WIN_CLOSED or event == 'キャンセル':
            break
        elif event == 'OK':
            print('ダウンロードURL', values[0])
            print('ファイル保存場所', values[1])

    window.close()

if __name__ == '__main__' :
    initGUI()
