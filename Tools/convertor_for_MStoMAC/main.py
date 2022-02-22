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
    read_file = open(path + file_name, 'r', encoding="shift-jis")

    # 書き出していく
    for data in read_file:
        print(data)
    read_file.close()

if __name__ == "__main__":
    main()