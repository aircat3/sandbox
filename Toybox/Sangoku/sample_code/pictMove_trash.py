import imp
from mimetypes import init
import PySimpleGUI as sg
import tkinter as tk
from PIL import Image, ImageTk

def initMap(canvas):
    line_color = 'black'

    # 1マスのサイズ(px)
    grid_size = 30
    # マスの縦・横数
    grid_width = 9
    grid_height = 9

    # マップの方眼紙の描画(線の数とマス数で2線のズレを修正)
    grid_width = grid_width + 2
    grid_height = grid_height + 2
    for x_pos in range(grid_width):
        for y_pos in range(grid_height):
            # ヨコ線描画
            canvas.tk_canvas.create_line(
                grid_size,
                y_pos * grid_size,
                (grid_width-1) * grid_size,
                y_pos * grid_size
            )
            # タテ線描画
            canvas.tk_canvas.create_line(
                x_pos * grid_size,
                grid_size,
                x_pos * grid_size,
                (grid_height-1) * grid_size
            )

def get_img_data(f, maxsize=(50, 50), first=False):
    """Generate image data using PIL
    """
    print("open file:", f)
    img = Image.open(f)
    img.thumbnail(maxsize)
    if first:  # tkinter is inactive the first time
        bio = io.BytesIO()
        img.save(bio, format="PNG")
        del img
        return bio.getvalue()
    return ImageTk.PhotoImage(img)

def initUnit(canvas):
    unit_image = ImageTk.PhotoImage(file = "unit.jpeg")

    # 画像の描画
    canvas.create_image(
        0,       # 画像表示位置(Canvasの中心)
        0,                   
        unit_image  # 表示画像データ
    )

def initGUI():

    # Windowテーマの設定
    #sg.theme_previewer()
    #sg.theme('dark grey 9')
    sg.theme('SystemDefault')
    
    # Windowレイアウトの設定
    canvas = sg.Canvas(size=(330,350))
    image_elem = sg.Image(pad=((0,0),(20,120)))
    layout = [
        [sg.Text('ダウンロードするURLを入力してね！')],
        [sg.Text('ダウンロード先URL', size=(10, 1)), sg.InputText()],
        [sg.Text('保存場所', size=(10, 1)), sg.InputText()],   
        [sg.Button('OK'), sg.Button('キャンセル')],
        [canvas],
        [image_elem]
    ]

    # Windowの生成
    window = sg.Window('サンプルプログラム', layout, finalize=True)

    # Mapの生成
    initMap(canvas)
    canvas.create_image(0, 0, './unit.png')
    #filename = './unit.png'
    #image_elem.update(data=get_img_data(filename))
    
    # イベント処理
    while True:
        event, values = window.read()
        if event == sg.WIN_CLOSED or event == 'キャンセル':
            break
        elif event == 'OK':
            print('ダウンロードURL', values[0])
            print('ファイル保存場所', values[1])

    window.close()

if __name__ == '__main__' :
    initGUI()
