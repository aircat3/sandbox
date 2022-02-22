# MS辞書データ(csv.txt)をMAC辞書データ(.plist)に変換する

def main():

    # 変換元ファイルを開く
    file = open('myfile.txt', 'r')

    for data in file:
        print(data)
    file.close()

if __name__ == "__main__":
    main()