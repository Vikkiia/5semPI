import tkinter as tk
from tkinter import ttk, messagebox
import math
import random

class SphereViewer:
    def __init__(self, root):
        self.root = root
        self.root.title("3D Sphere Viewer with Lighting Models")
        self.root.geometry("1200x700")
        
        # Параметры сферы
        self.sphere_radius = 100
        
        # Параметры камеры (в сферических координатах)
        self.camera_r = 300
        self.camera_phi = 30
        self.camera_theta = 60
        
        # Параметры источника света
        self.light_r = 200
        self.light_phi = 45
        self.light_theta = 30
        
        # Цвет источника света
        self.light_color = (255, 255, 255)
        
        # Модель отражения
        self.reflection_model = 0
        self.draw_sphere = False
        
        self.setup_ui()
    
    def setup_ui(self):
        # Создаем фреймы
        self.menu_frame = ttk.Frame(self.root, width=300)
        self.menu_frame.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=10)
        
        self.view_frame = ttk.Frame(self.root)
        self.view_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Заголовок меню
        title_label = ttk.Label(self.menu_frame, text="Управление сферой", 
                               font=('Arial', 14, 'bold'))
        title_label.pack(pady=10)
        
        subtitle_label = ttk.Label(self.menu_frame, text="Фигура: Шар", 
                                  font=('Arial', 12))
        subtitle_label.pack(pady=5)
        
        # Кнопки выбора модели
        model_frame = ttk.LabelFrame(self.menu_frame, text="Модель отражения")
        model_frame.pack(fill=tk.X, pady=10)
        
        ttk.Button(model_frame, text="Диффузная модель", 
                  command=lambda: self.set_reflection_model(0)).pack(fill=tk.X, pady=5)
        
        ttk.Button(model_frame, text="Зеркальная модель", 
                  command=lambda: self.set_reflection_model(1)).pack(fill=tk.X, pady=5)
        
        # Кнопки изменения параметров
        params_frame = ttk.LabelFrame(self.menu_frame, text="Параметры")
        params_frame.pack(fill=tk.X, pady=10)
        
        ttk.Button(params_frame, text="Изменить радиус шара", 
                  command=self.change_sphere_radius).pack(fill=tk.X, pady=5)
        
        ttk.Button(params_frame, text="Изменить камеру", 
                  command=self.change_camera_params).pack(fill=tk.X, pady=5)
        
        ttk.Button(params_frame, text="Изменить источник света", 
                  command=self.change_light_params).pack(fill=tk.X, pady=5)
        
        ttk.Button(params_frame, text="Изменить цвет света", 
                  command=self.change_light_color).pack(fill=tk.X, pady=5)
        
        ttk.Button(params_frame, text="Случайные параметры", 
                  command=self.random_params).pack(fill=tk.X, pady=5)
        
        # Область просмотра с черным фоном
        self.canvas = tk.Canvas(self.view_frame, bg='black', width=800, height=600)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        
        # Информационная панель
        self.info_frame = ttk.Frame(self.view_frame)
        self.info_frame.pack(fill=tk.X, pady=5)
        
        self.info_label = ttk.Label(self.info_frame, text="Выберите модель отражения в меню", 
                                   font=('Arial', 10), foreground='white', background='black')
        self.info_label.pack()
        
        # Привязываем событие изменения размера
        self.canvas.bind('<Configure>', self.on_canvas_configure)
    
    def set_reflection_model(self, model):
        self.reflection_model = model
        self.draw_sphere = True
        self.draw()
    
    def change_sphere_radius(self):
        dialog = SingleParameterDialog(self.root, "Изменение радиуса шара", 
                                      "Радиус шара:", self.sphere_radius)
        
        if dialog.result is not None:
            self.sphere_radius = dialog.result
            if self.draw_sphere:
                self.draw()
    
    def change_camera_params(self):
        dialog = ParameterDialog(self.root, "Параметры камеры", 
                               [("Расстояние (r):", self.camera_r),
                                ("Азимут (φ):", self.camera_phi),
                                ("Угол места (θ):", self.camera_theta)])
        
        if dialog.result:
            self.camera_r, self.camera_phi, self.camera_theta = dialog.result
            if self.draw_sphere:
                self.draw()
    
    def change_light_params(self):
        dialog = ParameterDialog(self.root, "Параметры источника света", 
                               [("Расстояние (r):", self.light_r),
                                ("Азимут (φ):", self.light_phi),
                                ("Угол места (θ):", self.light_theta)])
        
        if dialog.result:
            self.light_r, self.light_phi, self.light_theta = dialog.result
            if self.draw_sphere:
                self.draw()
    
    def change_light_color(self):
        color_dialog = ColorDialog(self.root, self.light_color)
        if color_dialog.result:
            self.light_color = color_dialog.result
            if self.draw_sphere:
                self.draw()
    
    def random_params(self):
        self.sphere_radius = random.randint(50, 150)
        self.camera_r = random.randint(200, 400)
        self.camera_phi = random.randint(0, 360)
        self.camera_theta = random.randint(10, 170)
        
        self.light_r = random.randint(150, 300)
        self.light_phi = random.randint(0, 360)
        self.light_theta = random.randint(10, 170)
        
        self.light_color = (random.randint(150, 255), 
                           random.randint(150, 255), 
                           random.randint(150, 255))
        
        if self.draw_sphere:
            self.draw()
    
    def on_canvas_configure(self, event):
        if self.draw_sphere:
            self.draw()
    
    def spherical_to_cartesian(self, r, phi, theta):
        phi_rad = math.radians(phi)
        theta_rad = math.radians(theta)
        
        x = r * math.sin(theta_rad) * math.cos(phi_rad)
        y = r * math.sin(theta_rad) * math.sin(phi_rad)
        z = r * math.cos(theta_rad)
        
        return x, y, z
    
    def calculate_light_intensity(self, point_x, point_y, point_z, normal_x, normal_y, normal_z, 
                                light_x, light_y, light_z, view_x, view_y, view_z):
        # Вектор от точки к источнику света
        to_light_x = light_x - point_x
        to_light_y = light_y - point_y
        to_light_z = light_z - point_z
        
        # Нормализуем вектор к свету
        length_light = math.sqrt(to_light_x**2 + to_light_y**2 + to_light_z**2)
        if length_light > 0:
            to_light_x /= length_light
            to_light_y /= length_light
            to_light_z /= length_light
        
        # Нормализуем нормаль
        normal_length = math.sqrt(normal_x**2 + normal_y**2 + normal_z**2)
        if normal_length > 0:
            normal_x /= normal_length
            normal_y /= normal_length
            normal_z /= normal_length
        
        # Вектор от точки к камере
        to_view_x = view_x - point_x
        to_view_y = view_y - point_y
        to_view_z = view_z - point_z
        
        # Нормализуем вектор к камере
        view_length = math.sqrt(to_view_x**2 + to_view_y**2 + to_view_z**2)
        if view_length > 0:
            to_view_x /= view_length
            to_view_y /= view_length
            to_view_z /= view_length
        
        if self.reflection_model == 0:  # Диффузная модель
            dot_product = normal_x * to_light_x + normal_y * to_light_y + normal_z * to_light_z
            intensity = max(0.2, min(1.0, dot_product))
            return intensity
        
        else:  # Зеркальная модель
            dot_nl = normal_x * to_light_x + normal_y * to_light_y + normal_z * to_light_z
            
            # Вектор отражения
            reflect_x = 2 * dot_nl * normal_x - to_light_x
            reflect_y = 2 * dot_nl * normal_y - to_light_y
            reflect_z = 2 * dot_nl * normal_z - to_light_z
            
            # Нормализуем отраженный вектор
            reflect_length = math.sqrt(reflect_x**2 + reflect_y**2 + reflect_z**2)
            if reflect_length > 0:
                reflect_x /= reflect_length
                reflect_y /= reflect_length
                reflect_z /= reflect_length
            
            # Косинус угла между отраженным лучом и направлением к камере
            dot_rv = reflect_x * to_view_x + reflect_y * to_view_y + reflect_z * to_view_z
            intensity = max(0.0, dot_rv)
            # Усиливаем эффект (чем больше степень, тем более "резким" будет блик)
            intensity = 0.3 + 0.7 * (intensity ** 8)
            return min(1.0, intensity)
    
    def project_3d_to_2d(self, x, y, z, center_x, center_y, scale):
        # Простая перспективная проекция
        factor = 500 / (500 + z)
        screen_x = center_x + int(x * factor * scale)
        screen_y = center_y + int(y * factor * scale)
        return screen_x, screen_y
    
    def rgb_to_hex(self, r, g, b):
        return f'#{r:02x}{g:02x}{b:02x}'
    
    def draw(self):
        self.canvas.delete("all")
        
        # Получаем размеры canvas
        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()
        
        if width <= 1 or height <= 1:
            return
        
        # Центр и масштаб
        center_x = width // 2
        center_y = height // 2
        scale = min(width, height) / 400
        
        # Получаем декартовы координаты камеры и источника света
        camera_x, camera_y, camera_z = self.spherical_to_cartesian(
            self.camera_r, self.camera_phi, self.camera_theta)
        light_x, light_y, light_z = self.spherical_to_cartesian(
            self.light_r, self.light_phi, self.light_theta)
        
        # Создаем полигоны для сферы (квады)
        segments = 30
        vertices = []
        polygons = []
        
        # Генерируем вершины
        for i in range(segments + 1):
            theta = math.pi * i / segments
            row_vertices = []
            for j in range(segments + 1):
                phi = 2 * math.pi * j / segments
                
                x = self.sphere_radius * math.sin(theta) * math.cos(phi)
                y = self.sphere_radius * math.sin(theta) * math.sin(phi)
                z = self.sphere_radius * math.cos(theta)
                
                # Нормаль в этой точке (для сферы с центром в начале координат)
                nx = x / self.sphere_radius
                ny = y / self.sphere_radius
                nz = z / self.sphere_radius
                
                # Рассчитываем интенсивность освещения
                intensity = self.calculate_light_intensity(
                    x, y, z, nx, ny, nz, 
                    light_x, light_y, light_z, 
                    camera_x, camera_y, camera_z
                )
                
                # Проецируем на 2D
                screen_x, screen_y = self.project_3d_to_2d(x, y, z, center_x, center_y, scale)
                
                # Вычисляем цвет
                r = int(self.light_color[0] * intensity)
                g = int(self.light_color[1] * intensity)
                b = int(self.light_color[2] * intensity)
                color = self.rgb_to_hex(r, g, b)
                
                row_vertices.append((screen_x, screen_y, color, z))
            
            vertices.append(row_vertices)
        
        # Создаем полигоны (квады)
        for i in range(segments):
            for j in range(segments):
                v1 = vertices[i][j]
                v2 = vertices[i][j+1]
                v3 = vertices[i+1][j+1]
                v4 = vertices[i+1][j]
                
                polygons.append((v1, v2, v3, v4))
        
        # Сортируем полигоны по глубине (простой z-sorting)
        polygons.sort(key=lambda poly: (poly[0][3] + poly[1][3] + poly[2][3] + poly[3][3]) / 4, reverse=True)
        
        # Рисуем полигоны
        for poly in polygons:
            points = [poly[0][0], poly[0][1], 
                     poly[1][0], poly[1][1],
                     poly[2][0], poly[2][1],
                     poly[3][0], poly[3][1]]
            
            # Используем средний цвет для полигона
            avg_color = poly[0][2]
            
            self.canvas.create_polygon(points, fill=avg_color, outline="", width=0)
        
        # Обновляем информацию
        model_name = "Диффузная" if self.reflection_model == 0 else "Зеркальная"
        info_text = (f"Модель: {model_name} | "
                    f"Радиус: {self.sphere_radius} | "
                    f"Камера: r={self.camera_r:.1f}, φ={self.camera_phi:.1f}°, θ={self.camera_theta:.1f}° | "
                    f"Источник: r={self.light_r:.1f}, φ={self.light_phi:.1f}°, θ={self.light_theta:.1f}° | "
                    f"Цвет: R={self.light_color[0]}, G={self.light_color[1]}, B={self.light_color[2]}")
        
        self.info_label.config(text=info_text)


class SingleParameterDialog:
    def __init__(self, parent, title, label_text, initial_value):
        self.result = None
        
        self.dialog = tk.Toplevel(parent)
        self.dialog.title(title)
        self.dialog.geometry("300x150")
        self.dialog.transient(parent)
        self.dialog.grab_set()
        
        ttk.Label(self.dialog, text=label_text).pack(pady=20)
        
        self.entry = ttk.Entry(self.dialog)
        self.entry.insert(0, str(initial_value))
        self.entry.pack(pady=10)
        
        button_frame = ttk.Frame(self.dialog)
        button_frame.pack(pady=20)
        
        ttk.Button(button_frame, text="OK", command=self.ok).pack(side=tk.LEFT, padx=10)
        ttk.Button(button_frame, text="Отмена", command=self.cancel).pack(side=tk.LEFT, padx=10)
        
        self.dialog.wait_window(self.dialog)
    
    def ok(self):
        try:
            value = float(self.entry.get())
            if value <= 0:
                messagebox.showerror("Ошибка", "Радиус должен быть положительным числом")
                return
            self.result = value
            self.dialog.destroy()
        except ValueError:
            messagebox.showerror("Ошибка", "Пожалуйста, введите числовое значение")
    
    def cancel(self):
        self.dialog.destroy()


class ParameterDialog:
    def __init__(self, parent, title, parameters):
        self.result = None
        
        self.dialog = tk.Toplevel(parent)
        self.dialog.title(title)
        self.dialog.geometry("300x200")
        self.dialog.transient(parent)
        self.dialog.grab_set()
        
        self.entries = []
        
        for i, (label, value) in enumerate(parameters):
            ttk.Label(self.dialog, text=label).grid(row=i, column=0, padx=10, pady=10, sticky=tk.W)
            entry = ttk.Entry(self.dialog)
            entry.insert(0, str(value))
            entry.grid(row=i, column=1, padx=10, pady=10)
            self.entries.append(entry)
        
        button_frame = ttk.Frame(self.dialog)
        button_frame.grid(row=len(parameters), column=0, columnspan=2, pady=20)
        
        ttk.Button(button_frame, text="OK", command=self.ok).pack(side=tk.LEFT, padx=10)
        ttk.Button(button_frame, text="Отмена", command=self.cancel).pack(side=tk.LEFT, padx=10)
        
        self.dialog.wait_window(self.dialog)
    
    def ok(self):
        try:
            self.result = [float(entry.get()) for entry in self.entries]
            self.dialog.destroy()
        except ValueError:
            messagebox.showerror("Ошибка", "Пожалуйста, введите числовые значения")
    
    def cancel(self):
        self.dialog.destroy()


class ColorDialog:
    def __init__(self, parent, current_color):
        self.result = None
        
        self.dialog = tk.Toplevel(parent)
        self.dialog.title("Выбор цвета источника света")
        self.dialog.geometry("300x300")
        self.dialog.transient(parent)
        self.dialog.grab_set()
        
        # Предопределенные цвета
        colors = [
            ("Белый", (255, 255, 255)),
            ("Красный", (255, 100, 100)),
            ("Зеленый", (100, 255, 100)),
            ("Синий", (100, 100, 255)),
            ("Желтый", (255, 255, 100)),
            ("Голубой", (100, 255, 255)),
            ("Пурпурный", (255, 100, 255)),
            ("Оранжевый", (255, 150, 50)),
        ]
        
        for i, (name, color) in enumerate(colors):
            frame = ttk.Frame(self.dialog)
            frame.pack(fill=tk.X, padx=20, pady=5)
            
            # Показываем цвет
            color_canvas = tk.Canvas(frame, width=30, height=30, bg=self.rgb_to_hex(*color))
            color_canvas.pack(side=tk.LEFT, padx=5)
            
            ttk.Button(frame, text=name, 
                      command=lambda c=color: self.select_color(c)).pack(side=tk.LEFT, padx=5)
        
        # Кнопка случайного цвета
        ttk.Button(self.dialog, text="Случайный цвет", 
                  command=self.random_color).pack(pady=10)
        
        self.dialog.wait_window(self.dialog)
    
    def rgb_to_hex(self, r, g, b):
        return f'#{r:02x}{g:02x}{b:02x}'
    
    def select_color(self, color):
        self.result = color
        self.dialog.destroy()
    
    def random_color(self):
        self.result = (random.randint(100, 255), 
                      random.randint(100, 255), 
                      random.randint(100, 255))
        self.dialog.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    app = SphereViewer(root)
    root.mainloop()