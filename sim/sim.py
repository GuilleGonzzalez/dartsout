import tkinter as tk
from tkinter import ttk

from dartboard import Dartboard
from api import Api

DEFAULT_SERVER_URL = "http://localhost:8000"
DEFAULT_BOARD_ID = 12345

FONT = "Helvetica"

ZONE_TEXT = {
    1: "SINGLE",
    2: "DOUBLE",
    3: "TRIPLE",
}

def update_board_id(*args):
    value = board_id_var.get()
    try:
        board_id = int(value, 16)
    except ValueError:
        print("Invalid dartboard ID. Must be a hexadecimal identifier")
        return

    print(f"New BOARD_ID: {value} ({board_id})")
    api.update_board_id(board_id)

root = tk.Tk()
root.title("Dartsout Simulator")
root.geometry("700x750")
root.configure(bg="#e6e6e6")

# Styles
style = ttk.Style()
style.theme_use("clam")

style.configure(
    "Blue.TButton",
    font=(FONT, 12, "bold"),
    padding=(10, 6),
    foreground="white",
    background="#1f6ae1",
    borderwidth=0
)

style.map(
    "Blue.TButton",
    background=[("active", "#1554b0")]
)

style.configure(
    "Board.TLabel",
    background="#e6e6e6",
    foreground="#333333",
    font=(FONT, 11, "bold")
)

style.configure(
    "Board.TEntry",
    font=(FONT, 11, "bold"),
    padding=4
)

# API
api = Api(DEFAULT_SERVER_URL, DEFAULT_BOARD_ID)

# Top bar
top_frame = tk.Frame(root, bg="#e6e6e6")
top_frame.pack(fill="x", pady=10, padx=10)

# ---- Left: BACK button
btn_back = ttk.Button(
    top_frame,
    text="BACK",
    style="Blue.TButton",
    command=api.back
)
btn_back.grid(row=0, column=0, sticky="w")

# ---- Center: Board ID
board_id_var = tk.StringVar(value=str(DEFAULT_BOARD_ID))
board_id_var.trace_add("write", update_board_id)

board_id_label = ttk.Label(
    top_frame,
    text="BOARD ID:",
    style="Board.TLabel"
)
board_id_label.grid(row=0, column=1, padx=(0, 6))

board_id_entry = ttk.Entry(
    top_frame,
    textvariable=board_id_var,
    width=8,
    justify="center",
    style="Board.TEntry"
)
board_id_entry.grid(row=0, column=2)

# ---- Right: NEXT PLAYER button
btn_next = ttk.Button(
    top_frame,
    text="NEXT PLAYER",
    style="Blue.TButton",
    command=api.next_player
)
btn_next.grid(row=0, column=3, sticky="e")

# Ajusta columnas para centrar el board ID
top_frame.grid_columnconfigure(0, weight=1)
top_frame.grid_columnconfigure(1, weight=0)
top_frame.grid_columnconfigure(2, weight=0)
top_frame.grid_columnconfigure(3, weight=1)

# Result label
result_label = tk.Label(
    root,
    text="CLICK THE DARTBOARD",
    fg="black",
    bg="#e6e6e6",
    font=(FONT, 14, "bold")
)
result_label.pack(pady=8)

# Canvas / Dartboard
canvas = tk.Canvas(
    root,
    bg="white",
    highlightthickness=0
)
canvas.pack(fill="both", expand=True, padx=10, pady=(0, 10))

dartboard = Dartboard(canvas)

def on_resize(event):
    dartboard.draw(event.width, event.height)

def on_click(event):
    num, zone, tag = dartboard.detect_hit(event.x, event.y)
    if num is None:
        return

    zone_text = ZONE_TEXT.get(zone, "")
    result_label.config(text=f"HIT: {num} {zone_text}")

    dartboard.highlight(tag)
    api.send_dart(num, zone)

canvas.bind("<Configure>", on_resize)
canvas.bind("<Button-1>", on_click)

root.mainloop()
