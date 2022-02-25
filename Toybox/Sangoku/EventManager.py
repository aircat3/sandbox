from collections import deque

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


def class_test():
    print('test')

if __name__ == '__main__':
    class_test()