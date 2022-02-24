from mimetypes import init
import PySimpleGUI as sg
import tkinter as tk

def initMap(canvas):
    line_color = 'red'

    # 1マスのサイズ(px)
    grid_size = 10
    # マスの縦・横数
    grid_width = 9
    grif_height = 9

    for x_pos in range(grid_width):
        for y_pos in range(grif_height):
            canvas.tk_canvas.create_line(
                x_pos,
                y_pos,
                x_pos * grid_size,
                y_pos * grid_size
                )

def initGUI():

    # Windowテーマの設定
    #sg.theme_previewer()
    #sg.theme('dark grey 9')
    sg.theme('SystemDefault')
    
    # Windowレイアウトの設定
    canvas = sg.Canvas(size=(200,200))
    layout = [
        [sg.Text('ダウンロードするURLを入力してね！')],
        [sg.Text('ダウンロード先URL', size=(10, 1)), sg.InputText()],
        [sg.Text('保存場所', size=(10, 1)), sg.InputText()],   
        [sg.Button('OK'), sg.Button('キャンセル')],
        [canvas]
    ]

    # Windowの生成
    window = sg.Window('サンプルプログラム', layout, finalize=True, element_justification='center')

    # Mapの生成
    initMap(canvas)

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
