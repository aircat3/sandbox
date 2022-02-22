# MS辞書データ(csv.txt)をMAC辞書データ(.plist)に変換する
import os

def main():

    # カレントディレクトリ取得
    path = os.getcwd()
    print("############")
    print("[Sys]" + path)
    print("############")

    # 変換元ファイルを開く
    file_name = '/sangokushi.txt'
    file = open(path + file_name, 'r', encoding="shift-jis")

    for data in file:
        print(data)
    file.close()

if __name__ == "__main__":
    main()