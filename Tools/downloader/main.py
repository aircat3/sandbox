import PySimpleGUI as sg
import requests
import os

# guiの初期設定

# URLの入力先
# ファイルの保存場所指定
# ダウンロードの実行と進行状況
# 

if __name__ == '__main__' :

    # Windowテーマの設定
    #sg.theme_previewer()
    sg.theme('dark grey 9')

    # Windowレイアウトの設定
    layout = [
        [sg.Text('ダウンロードするURLを入力してね！')],
        [sg.Text('ダウンロード先URL', size=(10, 1)), sg.InputText()],
        [sg.Text('保存場所', size=(10, 1)), sg.InputText()],
        [sg.Button('OK'), sg.Button('キャンセル')]
    ]

    # Windowの生成
    window = sg.Window('サンプルプログラム', layout, element_justification='center')

    # イベント処理
    while True:
        event, values = window.read()
        if event == sg.WIN_CLOSED or event == 'キャンセル':
            break
        elif event == 'OK':
            print('あなたが入力した値：', values[0])

    window.close()
