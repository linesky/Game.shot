import tkinter as tk
import random

class CircleShootingGame:
    def __init__(self, root):
        self.root = root
        self.root.title("Circle Shooting Game")
        self.canvas = tk.Canvas(root, width=640, height=640, bg='white')
        self.canvas.pack()
        
        self.grid_size = 16
        self.cell_size = 40
        self.circles = []

        self.create_grid()
        self.generate_circles(16)

        self.canvas.bind("<Button-1>", self.on_click)

    def create_grid(self):
        for i in range(self.grid_size + 1):
            self.canvas.create_line(i * self.cell_size, 0, i * self.cell_size, 640, fill='black')
            self.canvas.create_line(0, i * self.cell_size, 640, i * self.cell_size, fill='black')

    def generate_circles(self, count):
        while len(self.circles) < count:
            x = random.randint(0, self.grid_size - 1)
            y = random.randint(0, self.grid_size - 1)
            if (x, y) not in self.circles:
                self.circles.append((x, y))
                self.draw_circle(x, y)

    def draw_circle(self, x, y):
        x1 = x * self.cell_size + 5
        y1 = y * self.cell_size + 5
        x2 = (x + 1) * self.cell_size - 5
        y2 = (y + 1) * self.cell_size - 5
        self.canvas.create_oval(x1, y1, x2, y2, fill='blue', outline='black')

    def on_click(self, event):
        x = event.x // self.cell_size
        y = event.y // self.cell_size
        if (x, y) in self.circles:
            self.circles.remove((x, y))
            self.canvas.delete("all")
            self.create_grid()
            for cx, cy in self.circles:
                self.draw_circle(cx, cy)
            self.generate_circles(16)

if __name__ == "__main__":
    root = tk.Tk()
    game = CircleShootingGame(root)
    root.mainloop()

