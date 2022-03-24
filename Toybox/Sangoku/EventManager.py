from collections import deque
from concurrent.futures import thread
import time
import threading

class EventManager():
    def __init__(self):
        self.que = deque()
        self.que.append('StandBy')
    
    # 命令を追加
    def order(self, order):
        self.que.append(order)
    
    # 現在の命令を全て破棄し、優先実行
    def doit(self, order):
        self.que.clear()
        self.que.append(order)

    def evt_test(self):
        print('[Event]test event.')

    def execute_order(self):
        thread1 = threading.Thread(target=self.evt_Test)
        thread1.start()
    

def class_test():
    print('[Sys]EventManager function.')

if __name__ == '__main__':
    class_test()