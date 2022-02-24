import PySimpleGUI as sg
import tkinter as tk
from PIL import Image, ImageTk


class SimpleGUI():
    def __init__(self):
        title_name = 'Simulator'        # アプリタイトル
        window_theme = 'SystemDefault'  # Windowテーマの設定
        font_type = ('Arial',14)        # フォント指定

        #sg.theme_previewer()
        #sg.theme('dark grey 9')
        sg.theme(window_theme)
        
        # windowsレイアウト設定
        col1 = [
            [sg.Text('戦闘状況')],
            [sg.Multiline(
                default_text="hello\nworld\n!!\n", 
                font=font_type,
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
                '', font=font_type, size=(100, 30),
                enable_events=True, key='-INPUT-',
            )]
        ]
        layout = [
            [sg.Column(col1), sg.Column(col2)],
            [sg.Column(col3)],
            # inputでEnterキーを押された時用のダミーボタン
            [sg.Button('Submit', visible=False, bind_return_key=True)]
        ]

        self.window = sg.Window(title_name, layout, size=(500, 300), margins=(0,0))
    
    def getWindow(self):
        return self.window

def initGUI():

    simpleGUI = SimpleGUI()
    window = simpleGUI.getWindow()
    
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

    window.close()

if __name__ == '__main__' :
    initGUI()
