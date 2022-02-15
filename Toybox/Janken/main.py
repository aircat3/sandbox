from cgitb import reset
import random
from unittest import result

class PlayCharctor:
    def __init__(self, name):
        self.name = name
        self.hand = None
        # kill/death
        self.KD = {'win':0, 'lose':0, 'draw':0}
        # 戦闘回数
        self.battles = 0
        
    def countUp_KD(self, result):
        self.KD[result] += 1
        self.battles += 1

    def showKD(self):
        print('########################')
        print('[' + self.name + ']の戦績')
        print('戦闘数：' + str(self.battles))
        percent = self.KD['win']/self.battles*100
        print('勝率：' + str('{:.2f}'.format(percent)) + '%')
        print('勝利：' + str(self.KD['win']) + '回 ', end='')
        print('敗北：' + str(self.KD['lose']) + '回 ', end='')
        print('引分：' + str(self.KD['draw']) + '回 ')
        print('########################')

# ジャンケンの判定結果を返す。判定基準はplayer1に対して
def judge(player1: PlayCharctor, player2: PlayCharctor):
    result = player2.hand - player1.hand
    if(0 < result): result = 'win'
    elif(0 > result): result = 'lose'
    else: result = 'draw'
    return result

if __name__ == "__main__":

    # キャラクターの定義
    players = {}
    char_name = 'npc1'
    players.update({char_name:PlayCharctor(char_name)})
    char_name = 'npc2'
    players.update({char_name:PlayCharctor(char_name)})

    # グーチョキパーの定義
    GUTYPA = {
        1 : "グー",
        2 : "チョキ",
        0 : "パー"
    }

    # 対戦試行回数
    tryals = 500
    for battle_count in range(tryals):

        # キャラクターの手を決める
        players['npc1'].hand = random.randint(0, 2)
        players['npc2'].hand = random.randint(0, 2)

        # 勝敗決定
        result = judge(players['npc1'], players['npc2'])

        # 勝敗カウント
        players['npc1'].countUp_KD(result)
    
    players['npc1'].showKD()