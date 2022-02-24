import PySimpleGUI as sg
import ui

def main():
    simpleGUI = ui.SimpleGUI()

    window = simpleGUI.getWindow()

    while True:
        event, values = window.read()
        if event == sg.WIN_CLOSED or event == 'キャンセル':
            break

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

if __name__ == "__main__":
    main()