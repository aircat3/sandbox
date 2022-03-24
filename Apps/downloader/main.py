# GUIライブラリ
import PySimpleGUI as sg
# ファイル保存
import requests
import os
# ダウンロードの進捗リスト
from tqdm import tqdm
import sys, time

def initGUI():

    # Windowテーマの設定
    #sg.theme_previewer()
    sg.theme('dark grey 9')

    # Windowレイアウトの設定
    layout = [
        [sg.Text('ダウンロードするURLを入力してね！')],
        [sg.Text('ダウンロード先URL', size=(10, 1)), sg.InputText()],
        [sg.Text('保存場所', size=(10, 1)), sg.InputText()],
        
        # TODOディレクトリ選択ようのボタンを設置する

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

def intToZeroStr(num, zero_len):
    return str(num).zfill(zero_len)

def fileNumberCountUP(file_number):
    result = file_number + 1
    return result

def coolDown(times):
    limits = str(times * 100)
    for ct in range(times * 100):
        sys.stdout.write("[CT]\r%d /" % ct)
        sys.stdout.flush()
        time.sleep(0.01)
        
def download():

    header_url = 'https://www.mlit.go.jp/common/'
    file_number = 116950
    file_ext = '.pdf'

    # OPTION
    zero_len = 9            # sample '000116950'
    cool_time_s = 10        # Caution!!(Min10s)
    next_continue = 10      # download files

    for page_num in range(next_continue):

        # Ganarate File URL
        file_name = intToZeroStr(file_number, zero_len) + file_ext
        url = header_url + file_name
        file_size = int(requests.head(url).headers["content-length"])

        # Request and Save
        res = requests.get(url, stream=True)
        pbar = tqdm(total=file_size, unit="B", unit_scale=True)
        with open(file_name, 'wb') as file:
            for chunk in res.iter_content(chunk_size=1024):
                file.write(chunk)
                pbar.update(len(chunk))
            pbar.close()
        
        # Next file setting
        file_number = fileNumberCountUP(file_number)

        # Cool time
        coolDown(cool_time_s)


if __name__ == '__main__' :
    download()
    #initGUI()
