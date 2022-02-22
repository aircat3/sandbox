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

    # 変換先ファイルを作成
    file_name = '/sangokushi.plist'
    write_file = open(path + file_name, 'w', encoding='UTF-8')
    
    # .plistの先頭文を記述
    words = '<?xml version="1.0" encoding="UTF-8"?>'
    write_file.write(words + '\n')
    words = '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">'
    write_file.write(words + '\n')
    words = '<plist version="1.0">'
    write_file.write(words + '\n')
    words = '<array>'
    write_file.write(words + '\n')
    words = '<plist>'
    write_file.write(words + '\n')

    # 書き出していく
    for data in read_file:
        print(data)

    # .plistの終端文を記述
    words = '</array>'
    write_file.write(words + '\n')
    words = '</plist>'
    write_file.write(words + '\n')

    # ファイルクローズ
    read_file.close()
    write_file.close()

if __name__ == "__main__":
    main()