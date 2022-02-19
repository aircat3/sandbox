# GUIライブラリ
import PySimpleGUI as sg
# ファイル保存
import requests
import os
# ダウンロードの進捗リスト
from tqdm import tqdm

def initGUI():

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
            print('ダウンロードURL', values[0])
            print('ファイル保存場所', values[1])

    window.close()

def main():

    file_url = "https://images-fe.ssl-images-amazon.com/images/I/51mBstgMBlL._SY291_BO1,204,203,200_QL40_ML2_.jpg"
    file_size = int(requests.head(file_url).headers["content-length"])

    res = requests.get(file_url, stream=True)
    pbar = tqdm(total=file_size, unit="B", unit_scale=True)
    with open("pict.jpg", 'wb') as file:
        for chunk in res.iter_content(chunk_size=1024):
            file.write(chunk)
            pbar.update(len(chunk))
        pbar.close()


if __name__ == '__main__' :
    #main()
    #initGUI()