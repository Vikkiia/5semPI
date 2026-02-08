import tkinter as tk
from tkinter import simpledialog
import math
import numpy as np

class CPlot3D:
    def __init__(self):
        # Начальные значения координат камеры по умолчанию
        self.r = 10      # расстояние до камеры
        self.theta = 45  # азимут θ (горизонтальный угол)
        self.phi = 45    # угол подъема φ (вертикальный угол)
        self.scale = 60
        self.center_x = 600
        self.center_y = 450
        
    def set_camera(self, theta, phi, r=None):
        self.theta = theta
        self.phi = phi
        if r is not None:
            self.r = r
        
    def get_camera_position(self):
        """Преобразует сферические координаты камеры в декартовы"""
        theta_rad = math.radians(self.theta)
        phi_rad = math.radians(self.phi)
        
        x = self.r * math.sin(phi_rad) * math.cos(theta_rad)
        y = self.r * math.sin(phi_rad) * math.sin(theta_rad)
        z = self.r * math.cos(phi_rad)
        
        return x, y, z
        
    def project_point(self, x, y, z):
        """Аксонометрическая проекция 3D точки в 2D"""
        theta_rad = math.radians(self.theta)
        phi_rad = math.radians(self.phi)
        
        # Правильное преобразование для аксонометрической проекции
        # Сначала поворот вокруг оси Y (азимут theta)
        x_rot = x * math.cos(theta_rad) + z * math.sin(theta_rad)
        z_rot = -x * math.sin(theta_rad) + z * math.cos(theta_rad)
        
        # Затем поворот вокруг оси X (угол подъема phi)
        y_rot = y * math.cos(phi_rad) - z_rot * math.sin(phi_rad)
        z_final = y * math.sin(phi_rad) + z_rot * math.cos(phi_rad)
        
        # Проекция на плоскость (ортографическая)
        x_2d = self.center_x + x_rot * self.scale
        y_2d = self.center_y - y_rot * self.scale
        
        return x_2d, z_final  # возвращаем z_final для сортировки по глубине
        
    def elliptic_paraboloid(self, u, v):
        """Эллиптический параболоид: z = x^2 + y^2"""
        x = u
        y = v
        z = u**2 + v**2
        return x, y, z
        
    def hyperbolic_paraboloid(self, u, v):
        """Гиперболический параболоид: z = x^2 - y^2"""
        x = u
        y = v
        z = u**2 - v**2
        return x, y, z
        
    def hemisphere(self, u, v):
        """Верхняя полусфера: z = sqrt(1 - x^2 - y^2)"""
        x = u
        y = v
        
        # Вычисляем радиус
        r = math.sqrt(u**2 + v**2)
        
        # Если точка внутри единичной окружности
        if r <= 1:
            z = math.sqrt(1 - r**2)  # z = sqrt(1 - r^2)
        else:
            z = 0  # точка вне полусферы
            
        return x, y, z


class SurfaceApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Лабораторная работа №8 - Поверхности второго порядка")
        self.root.geometry("1500x1000")
        
        self.canvas = tk.Canvas(root, width=1500, height=1000, bg="white")
        self.canvas.pack()
        
        self.plot3d = CPlot3D()
        self.current_surface = None
        
        self.create_menu()
        self.update_camera_display()
        
        # Выводим информацию о начальных значениях
        print(f"Начальные значения координат камеры:")
        print(f"r = {self.plot3d.r}")
        print(f"θ = {self.plot3d.theta}°")
        print(f"φ = {self.plot3d.phi}°")
        
    def create_menu(self):
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        # Меню фигур
        figure_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Фигура", menu=figure_menu)
        
        figure_menu.add_command(label="Эллиптический параболоид",
                                command=lambda: self.show_surface("elliptic"))
        figure_menu.add_command(label="Гиперболический параболоид",
                                command=lambda: self.show_surface("hyperbolic"))
        figure_menu.add_command(label="Полусфера",
                                command=lambda: self.show_surface("hemisphere"))
        
        # Меню положения камеры
        camera_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Положение камеры", menu=camera_menu)
        
        camera_menu.add_command(label="По умолчанию", 
                                command=self.set_default_camera)
        camera_menu.add_command(label="Изменить", 
                                command=self.change_camera_position)
        
    def update_camera_display(self):
        """Обновляет отображение текущих координат камеры"""
        self.canvas.delete("camera_info")
        self.canvas.create_text(100, 20, text=f"θ={self.plot3d.theta}°, φ={self.plot3d.phi}°", 
                                font=("Arial", 12), tags="camera_info")
        
    def set_default_camera(self):
        """Устанавливает положение камеры по умолчанию"""
        self.plot3d.set_camera(45, 45, 10)  # r=10, θ=45°, φ=45°
        if self.current_surface:
            self.show_surface(self.current_surface)
            
    def change_camera_position(self):
        """Изменяет положение камеры через диалоговое окно"""
        try:
            theta = simpledialog.askfloat("Угол theta", "Введите угол theta (в градусах):", 
                                        initialvalue=self.plot3d.theta)
            phi = simpledialog.askfloat("Угол phi", "Введите угол phi (в градусах):", 
                                        initialvalue=self.plot3d.phi)
            
            r = 10
            if theta is not None and phi is not None and r is not None:
                self.plot3d.set_camera(theta, phi, r)
                if self.current_surface:
                    self.show_surface(self.current_surface)
                    
        except ValueError:
            pass
            
    def generate_surface_points(self, surface_type, steps=25):
        """Генерирует точки поверхности"""
        points = []
        
        if surface_type == "elliptic":
            # Эллиптический параболоид: -2 <= x,y <= 2
            u_range = np.linspace(-2, 2, steps)
            v_range = np.linspace(-2, 2, steps)
            for u in u_range:
                row = []
                for v in v_range:
                    x, y, z = self.plot3d.elliptic_paraboloid(u, v)
                    x2d, depth = self.plot3d.project_point(x, y, z)
                    y2d = self.plot3d.center_y - depth * self.plot3d.scale
                    row.append((x2d, y2d, depth))
                points.append(row)
                
        elif surface_type == "hyperbolic":
            # Гиперболический параболоид: -2 <= x,y <= 2
            u_range = np.linspace(-2, 2, steps)
            v_range = np.linspace(-2, 2, steps)
            for u in u_range:

                row = []
                for v in v_range:
                    x, y, z = self.plot3d.hyperbolic_paraboloid(u, v)
                    x2d, depth = self.plot3d.project_point(x, y, z)
                    y2d = self.plot3d.center_y - depth * self.plot3d.scale
                    row.append((x2d, y2d, depth))
                points.append(row)
                
        elif surface_type == "hemisphere":
            # Полусфера: -1 <= x,y <= 1
            u_range = np.linspace(-1, 1, steps)
            v_range = np.linspace(-1, 1, steps)
            for u in u_range:
                row = []
                for v in v_range:
                    x, y, z = self.plot3d.hemisphere(u, v)
                    x2d, depth = self.plot3d.project_point(x, y, z)
                    y2d = self.plot3d.center_y - depth * self.plot3d.scale
                    row.append((x2d, y2d, depth))
                points.append(row)
                
        return points
        
    def painter_algorithm_draw(self, surface_type):
        """Алгоритм художника для удаления невидимых граней с треугольниками"""
        points = self.generate_surface_points(surface_type)
        
        # Создаем треугольники вместо четырехугольников
        triangles = []
        for i in range(len(points) - 1):
            for j in range(len(points[i]) - 1):
                # Берем 4 точки квадрата
                p1 = points[i][j]      # (x, y, depth)
                p2 = points[i][j+1]    # (x, y, depth)
                p3 = points[i+1][j+1]  # (x, y, depth)
                p4 = points[i+1][j]    # (x, y, depth)
                
                # Разбиваем четырехугольник на ДВА треугольника
                # Треугольник 1: p1-p2-p3
                avg_depth1 = (p1[2] + p2[2] + p3[2]) / 3
                triangles.append({
                    'points': [p1, p2, p3],
                    'depth': avg_depth1
                })
                
                # Треугольник 2: p1-p3-p4
                avg_depth2 = (p1[2] + p3[2] + p4[2]) / 3
                triangles.append({
                    'points': [p1, p3, p4],
                    'depth': avg_depth2
                })
        
        # Сортируем треугольники по глубине (от дальних к ближним)
        triangles.sort(key=lambda t: t['depth'])
        
        # Рисуем треугольники в порядке от дальних к ближним
        for triangle in triangles:
            points_2d = [(p[0], p[1]) for p in triangle['points']]
            
            # Определяем цвет в зависимости от глубины
            if surface_type == "elliptic":
                depth_factor = max(0, min(1, (triangle['depth'] + 2) / 6))
                blue = int(100 + 155 * depth_factor)
                color = f'#0000{blue:02x}'
            elif surface_type == "hyperbolic":
                depth_factor = max(0, min(1, (triangle['depth'] + 2) / 6))
                red = int(100 + 155 * depth_factor)
                color = f'#{red:02x}0000'
            else:  # hemisphere
                depth_factor = max(0, min(1, (triangle['depth'] + 1) / 3))
                green = int(100 + 155 * depth_factor)
                color = f'#00{green:02x}00'
            
            # Рисуем заполненный треугольник
            self.canvas.create_polygon(points_2d, fill=color, outline="white", width=1)
            
    def show_surface(self, surface_type):
        """Отображает выбранную поверхность"""
        self.current_surface = surface_type
        self.canvas.delete("all")
        self.update_camera_display()
        
        # Рисуем поверхность с использованием алгоритма художника
        self.painter_algorithm_draw(surface_type)
        
        # Добавляем подпись
        titles = {
            "elliptic": "Эллиптический параболоид: z = x² + y²",
            "hyperbolic": "Гиперболический параболоид: z = x² - y²", 
            "hemisphere": "Верхняя полусфера: z = √(1 - x² - y²)"
        }
        
        self.canvas.create_text(400, 550, text=titles[surface_type], 
                                font=("Arial", 14), fill="black")


if __name__ == "__main__":
    root = tk.Tk()
    app = SurfaceApp(root)
    root.mainloop()
