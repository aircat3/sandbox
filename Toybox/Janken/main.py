from cgitb import reset
import random

class PlayCharctor:
    def __init__(self, name):
        self.name = name
        self.hand = None
        self.cnt_win = 0
        self.cnt_lose =0
    def countUp_win(self):
        self.cnt_win += 1
    def countUp_lose(self):
        self.cnt_lose += 1

# ジャンケンの判定結果を返す。判定基準はplayer1に対して
def judge(player1: PlayCharctor, player2: PlayCharctor):
    result = player2.hand - player1.hand
    if(0 < result): result = 'win'
    elif(0 > result): result = 'lose'
    else: result = 'draw'

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

    # 勝敗定義
    WIN = 1
    DRAW = 0
    LOSE = -1
    JUDGE = [
        [DRAW, WIN,  LOSE], # P1 ぐー
        [LOSE, DRAW, WIN],  # P1 ちょき
        [WIN,  LOSE, DRAW]  # P1 ぱー
    ]

    # 対戦試行回数
    tryals = 10
    for battle_count in range(tryals):

        # キャラクターの手を決める
        non_player_1 = random.randint(0, 2)
        non_player_2 = random.randint(0, 2)

        # 勝敗決定
        #print("[Player1:" + GUTYPA[non_player_1] + "]")
        #print("[Player2:" + GUTYPA[non_player_2] + "]")
        result = JUDGE[non_player_1][non_player_2]

        # Player2の勝利数カウント
        if result == LOSE : win_count += 1

    print("Player2の勝利数:" + str(win_count))

    """
    if result == WIN:
        print("Player1の勝利です。")
    elif result == LOSE:
        print("Player2の勝利です。")
    elif result == DRAW:
        print("引き分けです。")
    """
    
    
