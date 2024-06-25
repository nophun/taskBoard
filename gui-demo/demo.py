from tkinter import *
from tkinter.ttk import Label, Style, Treeview

border = 2
card_w = 250
card_h = 122
spacing = card_h + border + 6

class Card: 
    def __init__(self, master, position) -> None:
        global border
        # Instantiating master i.e toplevel Widget
        self.master = master
        # Instantiating Style class 
        self.style = Style(self.master)

        self.canvas = Canvas(self.master, width=card_w, height=card_h, borderwidth=1, relief="solid")
        self.canvas.place(x=75, y=position*spacing)
        self.canvas.create_line(25, 110, 185, 110, fill="grey", width=3, dash=(40,1))
        self.canvas.create_oval(10, 105, 20, 115, outline="grey", width=3)
        self.canvas.create_oval(190, 105, 200, 115, fill="grey", outline="grey", width=3)

        # Configuring Custom Style
        self.style.configure("Body.TLabel", font=('Molga Medium', 9))
        self.style.configure("Counter.TLabel", font=('Molga Medium', 12))
        self.style.configure("Title.TLabel", font=('Pockota Bold', 20))


    def set_title(self, title_str):
        Label(self.canvas, text=title_str, style="Title.TLabel", borderwidth=0, relief="").place(x=10, y=4)

    def set_class(self, classification_str):
        Label(self.canvas, text=classification_str, style="Body.TLabel", borderwidth=0, relief="").place(x=190, y=10)

    def set_description(self, description_str):
        Label(self.canvas, text=description_str, style="Body.TLabel", borderwidth=0, relief="").place(x=5, y=39)

    def set_counter(self, counter, maximum):
        Label(self.canvas, text="%d/%d" % (counter, maximum), style="Counter.TLabel", borderwidth=0, relief="").place(x=215, y=95)

class App:
    def __init__(self, master, number_of_cards) -> None:
        global border
        self.master = master
        self.canvas = Canvas(self.master, width=400, height=spacing*number_of_cards)
        for i in range(number_of_cards - 1):
            self.canvas.create_line(0, (spacing - border/2) + i * spacing,
                                   400, (spacing - border/2) + i * spacing,
                                   fill="grey", width=border)
            Label(self.canvas, text="slot %d" % (i + 1)).place(x=10, y=50 + i * spacing)
        Label(self.canvas, text="slot %d" % (number_of_cards)).place(x=10, y=50 + (number_of_cards - 1) * spacing)
        self.canvas.pack()
        self.cards = []
        for i in range(number_of_cards):
            card = Card(self.canvas, i)
            card.set_title("Title1234 %d" % (i+1))
            card.set_class("[CLASS]")
            card.set_description("I have a font-size of 10.\nI also have another row.\nHowever, third does not fit...")
            card.set_counter(1, 4)
            self.cards.append(card)

class Form:
    def __init__(self, master, count, real):
        self.master = master
        # form_window.geometry("1000x800")
        self.master.columnconfigure(2, weight=10)
        self.master.columnconfigure(1, weight=10)
        self.canvas = Canvas(self.master)
        self.slots = []

        for row in range(count):
            Card = {}

            Cn = Canvas(self.canvas)
            Cn.grid(row=row+1, column=0, padx=10)
            Cn.bind("<ButtonPress-1>", self.btDown)
            Cn.bind("<ButtonRelease-1>", self.btUp)
            if (count - row <= real):
                Cn.configure(bg='grey')
            Card["canvas"] = Cn

            Label(Cn, text="Slot {}".format(row)).grid(row=0, column=0, columnspan=2)
            # Title
            T = Text(Cn, height=1, width=12)
            T.grid(row=1, column=0, padx=10)
            T.insert(1.0, "Title: {}".format(row)) 
            Card["title"] = T

            # Description
            D = Text(Cn, height=3, width=32)
            D.grid(row=1, column=1, rowspan=2, padx=10, pady=10)
            D.insert(1.0, "Desc: {}".format(row)) 
            Card["desc"] = D

            # Class
            C = Text(Cn, height=1, width=12)
            C.grid(row=2, column=0, padx=10)
            C.insert(1.0, "Class: {}".format(row)) 
            Card["class"] = C

            self.slots.append(Card)
        self.canvas.pack(side=LEFT)

    def btDown(self, event):
        x = event.x_root - self.canvas.winfo_rootx()
        y = event.y_root - self.canvas.winfo_rooty()
        c,r = self.canvas.grid_location(x, y)
        if (r > 0):
            self.start_row = r - 1
            print("press {}".format(r))

    def btUp(self, event):
        x = event.x_root - self.canvas.winfo_rootx()
        y = event.y_root - self.canvas.winfo_rooty()
        c,r = self.canvas.grid_location(x, y)
        if (r > 0):
            stop_row = r - 1
            print("release {}".format(r))
            if (self.start_row != stop_row):
                self.move_cards(self.start_row, stop_row)

    def move_cards(self, start, stop):
        if (start > stop):
            for i in range(stop, start):
                idx = stop+start-i
                self.swap_cards(idx, idx-1)
        else:
            for idx in range(start, stop):
                self.swap_cards(idx, idx+1)

    def swap_cards(self, source, target):
        print("drag {} to {}".format(source, target))
        temp = {}
        target_title = self.slots[target]["title"].get(1.0, END)
        target_desc = self.slots[target]["desc"].get(1.0, END)
        target_class = self.slots[target]["class"].get(1.0, END)
        self.slots[target]["title"].delete(1.0, END)
        self.slots[target]["desc"].delete(1.0, END)
        self.slots[target]["class"].delete(1.0, END)
        self.slots[target]["title"].insert(1.0, self.slots[source]["title"].get(1.0, END))
        self.slots[target]["desc"].insert(1.0, self.slots[source]["desc"].get(1.0, END))
        self.slots[target]["class"].insert(1.0, self.slots[source]["class"].get(1.0, END))
        self.slots[source]["title"].delete(1.0, END)
        self.slots[source]["desc"].delete(1.0, END)
        self.slots[source]["class"].delete(1.0, END)
        self.slots[source]["title"].insert(1.0, target_title)
        self.slots[source]["desc"].insert(1.0, target_desc)
        self.slots[source]["class"].insert(1.0, target_class)

if __name__ == "__main__": 
    C = 6
    # demo_window = Tk()
    # demo_window.title("Demo")
    # demo_window.geometry("400x%d" % (C*spacing - 2)) # 400x648
    # app = App(demo_window, C)

    form_window = Tk()
    form_window.title("Form")
    form = Form(form_window, 10, 6)

    # Mainloop which will cause this toplevel to run infinitely
    # demo_window.mainloop()
    form_window.mainloop()