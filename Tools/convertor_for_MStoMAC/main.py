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
    words = '<?xml version="1.0" encoding="UTF-8"?>\n'
    words = words + '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n'
    words = words + '<plist version="1.0">\n'
    words = words + '<array>\n'
    write_file.write(words)

    # 書き出していく
    for data in read_file:
        # MSのデータを分割
        data_list = data.split()
        # MACの書式に整形
        words = '\t<dict>\n'
        words = words + '\t\t<key>phrase</key>\n'
        words = words + '\t\t<string>' + data_list[1] + '</string>' + '\n'
        words = words + '\t\t<key>shortcut</key>\n'
        words = words + '\t\t<string>' + data_list[0] + '</string>' + '\n'
        words = words + '\t</dict>\n'
        write_file.write(words)

    # .plistの終端文を記述
    words = '</array>\n'
    words =  words + '</plist>\n'
    write_file.write(words)

    # ファイルクローズ
    read_file.close()
    write_file.close()

if __name__ == "__main__":
    main()