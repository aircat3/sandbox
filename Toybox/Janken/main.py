import random

if __name__ == "__main__":

    # キャラクターの定義
    non_player_1 = 0
    non_player_2 = 0

    # グーチョキパーの定義
    GUTYPA = {
        0 : "グー",
        1 : "チョキ",
        2 : "パー"
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

    # キャラクターの手を決める
    non_player_1 = random.randint(0, 2)
    non_player_2 = random.randint(0, 2)

    # 勝敗決定
    print("[Player1:" + GUTYPA[non_player_1] + "]")
    print("[Player2:" + GUTYPA[non_player_2] + "]")
    result = JUDGE[non_player_1][non_player_2]
    if result == WIN:
        print("Player1の勝利です。")
    elif result == LOSE:
        print("Player2の勝利です。")
    elif result == DRAW:
        print("引き分けです。")