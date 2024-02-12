import tkinter as tk
import tkinter.ttk as ttk


def bDown(event):
    print ("button down ---------------------------")
    global col_from
    tv = event.widget
    col = tv.identify_row(event.y)
    if col != '':
        col = int(col[4:])
    col_from = col
    print ("selected index {}".format(col))


def bUp(event):
    print ("button up ---------------------------")
    tv = event.widget
    print(tv["columns"])
    col = tv.identify_row(event.y)
    if col != '':
        col = int(col[4:])
    col_to = col

    if col_from != col_to:
        dcols = list(tv["displaycolumns"])
        if dcols[0] == "#all":
            dcols = list(tv["columns"])
        print ("Display Columns")
        print (dcols)
        print ("move from {} to {}".format(col_from, col_to))

        if col_from > col_to:
            dcols.insert(col_to, dcols[col_from])
            dcols.pop(col_from + 1)
        else:
            dcols.insert(col_to + 1, dcols[col_from])
            dcols.pop(col_from)
        tv.config(displaycolumns=dcols)
        print (dcols)


# Variable to hold initial choice of column to move
col_from = 0

root = tk.Tk()

# List of columns
columns = ["A", "B", "C", "D", "E", "F", "G"]

# Create treeview with columns. Display all columns
tree = ttk.Treeview(columns=columns)#, displaycolumns=columns)

# Set headers
for col in columns:
    tree.heading(col, text=col)

# Make first column (tree node) very narrow
tree.column("#0", stretch=tk.N, minwidth=10, width=10)

# insert some items into the tree
for i in range(10):
    tree.insert('', 'end',iid='line%i' % i, values=(i, i+10, i+20, i+30, i+40, i+50, i+60))

tree.grid()
tree.bind("<ButtonPress-1>", bDown)
tree.bind("<ButtonRelease-1>",bUp, add='+')

root.mainloop()