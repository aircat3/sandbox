import PySimpleGUI as sg

sg.theme('dark grey 9')

# Windowレイアウトの設定
layout = [
    [sg.Text('ここは1行目')],
    [sg.Text('ここは2行目：文字を入力してください'), sg.InputText()],
    [sg.Button('OK'), sg.Button('キャンセル')]
]

# Windowの生成
window = sg.Window('サンプルプログラム', layout)

# イベント処理
while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED or event == 'キャンセル':
        break
    elif event == 'OK':
        print('あなたが入力した値：', values[0])

window.close()