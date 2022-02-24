from ctypes import alignment
import PySimpleGUI as sg
import tkinter as tk
from PIL import Image, ImageTk

def initGUI():

    # Windowテーマの設定
    #sg.theme_previewer()
    #sg.theme('dark grey 9')
    sg.theme('SystemDefault')
    
    # Windowレイアウトの設定
    col1 = [
        [sg.Text('戦闘状況')],
        [sg.Multiline(
            default_text="hello\nworld\n!!\n", 
            font=('Arial',14),
            size=(40,12),
            disabled=True,
            key='-LOG-')
        ]
    ]
    col2 = [[sg.Frame('部隊状況',[
        [sg.Text('部隊１')],
        [sg.Text('部隊２', size=(50,15))],
    ])]]
    col3 = [
        [sg.Text('コマンド')],
        [sg.InputText(
            '', font=('Arial',14), size=(100, 30),
            enable_events=True, key='-INPUT-',
        )]
    ]
    layout = [
        [sg.Column(col1), sg.Column(col2)],
        [sg.Column(col3)],
        # inputでEnterキーを押された時用のダミーボタン
        [sg.Button('Submit', visible=False, bind_return_key=True)]
    ]

    # Windowの生成
    window = sg.Window('サンプルプログラム', layout, size=(500, 300), margins=(0,0))
    
    # イベント処理
    while True:
        event, values = window.read()
        if event == sg.WIN_CLOSED or event == 'キャンセル':
            break
        #elif len(values['-INPUT-']) and values['-INPUT-'][-1] not in ('0123456789'):
        #    print(values['-INPUT-'])

        # Enterキー処理
        elif event == 'Submit':
            input_text = window['-INPUT-'].get()
            print('You have submited %s'% input_text)
            # ログにテキストを追加し、入力エリアをクリア
            window['-LOG-'].print(input_text)
            window['-INPUT-'].update('')

        elif event == 'OK':
            print('ダウンロードURL', values[0])
            print('ファイル保存場所', values[1])

    window.close()

if __name__ == '__main__' :
    initGUI()
