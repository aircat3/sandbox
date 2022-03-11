import PySimpleGUI as sg
from EventManager import EventManager
import ui

def main():
    simpleGUI = ui.SimpleGUI()

    window = simpleGUI.getWindow()

    while True:
        event, values = window.read(timeout=1000,timeout_key='-TIMEOUT-')
        if event == sg.WIN_CLOSED or event == 'キャンセル':
            break

        # Enterキー処理
        elif event == 'Submit':
            input_text = window['-INPUT-'].get()

            cmd_event = EventManager()
            cmd_event.order(input_text)

            print('You have submited %s'% input_text)
            # ログにテキストを追加し、入力エリアをクリア
            simpleGUI.logMessage(input_text)
            window['-INPUT-'].update('')

        elif event == 'OK':
            print('ダウンロードURL', values[0])
            print('ファイル保存場所', values[1])

        elif event == '-TIMEOUT-':
            time = simpleGUI.getTime()
            window['-TIMER-'].update(time)

    window.close()

if __name__ == "__main__":
    main()