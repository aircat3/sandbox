# システム系
from asyncio import futures
from collections import deque
import threading, time
from concurrent.futures import ThreadPoolExecutor
# UI系統
import PySimpleGUI as sg
from ui import SimpleGUI
# 部下
import EventEngineer as ee

class EventManager():
    def __init__(self):
        # windowの生成
        self.simplegui = SimpleGUI()
        self.app_window = self.simplegui.getWindow()

        # キューの生成
        self.que = deque()
        self.que.append('StandBy')

        # スレッド管理
        self.executor = ThreadPoolExecutor(max_workers=3)

    # 命令を追加
    def order(self, order):
        self.que.append(order)
    
    # 現在の命令を全て破棄し、優先実行
    def doit(self, order):
        self.que.clear()
        self.que.append(order)

    def evt_test(self):
        time.sleep(2)
        self.window.logPrint('[Event]test event.')

    def execute_order(self):
        self.future = self.executor.submit(ee.Message.test())

    def len_order(self):
        return len(self.que)
    
    def check_order(self):
        if self.future.done:
            return 'done'
        elif self.future.running:
            return 'running'
        else:
            return 'Error "check_order".'

    # メインのウィンドウ処理
    def execute(self):
        simplegui = self.simplegui
        app_window = self.app_window
        while True:
            event, values = app_window.read(timeout=1000,timeout_key='-TIMEOUT-')
            if event == sg.WIN_CLOSED or event == 'キャンセル':
                break

            # Enterキー処理
            elif event == 'Submit':
                input_text = app_window['-INPUT-'].get()
                self.order(input_text)

                print('You have submited %s'% input_text)
                # ログにテキストを追加し、入力エリアをクリア
                simplegui.logMessage(input_text)
                app_window['-INPUT-'].update('')

            elif event == 'OK':
                print('ダウンロードURL', values[0])
                print('ファイル保存場所', values[1])

            elif event == '-TIMEOUT-':
                time = simplegui.getTime()
                app_window['-TIMER-'].update(time)

        app_window.close()

def class_test():
    print('[Sys]EventManager function.')

if __name__ == '__main__':
    class_test()