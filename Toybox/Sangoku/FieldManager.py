from collections import deque

class FieldManager():
    def __init__(self):
        self.map = {
            'a1': (0, 0), 'a2': (1, 0), 'a3': (2, 0),
            'b1': (0, 1), 'b2': (1, 1), 'b3': (2, 1),
            'c1': (0, 2), 'c2': (1, 2), 'c3': (2, 2),
        }

    def getPoint(self, location):
        print(self.map[location])
    
    def getLocation(self, point):
        # y軸を数値→アルファベット変換
        alphabets = list("abcdefghijklmnopqrstuvwxyz")
        word = alphabets[point[1]]
        # x軸を文字化、1始まりなので+1
        number = str(point[0] + 1)

        return word + number

def class_test():
    fManager = FieldManager()
    
    hoge = fManager.getLocation((2,8))
    print(hoge)

if __name__ == '__main__':
    class_test()