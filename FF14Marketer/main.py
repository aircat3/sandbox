import PySimpleGUI as sg

# Windowテーマの設定
#sg.theme_previewer()
sg.theme('dark grey 9')

# Windowレイアウトの設定
layout = [
    [sg.Text('マーケット記録画面')],
    [sg.Text('アイテム名', size=(10, 1)), sg.InputText()],
    [sg.Text('HQ品', size=(10, 1)), sg.InputText()],
    [sg.Text('単価', size=(10, 1)), sg.InputText()],
    [sg.Text('個数', size=(10, 1)), sg.InputText()],
    [sg.Text('購入者名', size=(10, 1)), sg.InputText()],
    [sg.Text('取引時刻', size=(10, 1)), sg.InputText()],
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