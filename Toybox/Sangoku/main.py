import PySimpleGUI as sg
from EventManager import EventManager
import ui

def main():
    app = EventManager()
    app.execute()

if __name__ == "__main__":
    main()