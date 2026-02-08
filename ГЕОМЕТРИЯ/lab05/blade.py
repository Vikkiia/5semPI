import tkinter as tk
import math
import sys

class BladeApp:
    def __init__(self, root):
        print("Инициализация приложения...")
        self.root = root
        self.root.title("Лопасть - Лабораторная работа")
        self.root.geometry("800x600")
        
        # Холст для рисования
        self.canvas = tk.Canvas(root, width=800, height=600, bg="white")
        self.canvas.pack()
        
        # Параметры лопастей (по умолчанию)
        self.base_width = 10   # Узкое основание у центра
        self.tip_width = 60    # Широкий конец лопасти
        self.h = 200           # Длина лопасти
        self.angle = 0
        self.rotating = False
        self.blades_visible = False  # Флаг, показывающий, отображены ли лопасти
        
        self.rotation_direction = 1  # -1 = против часовой, 1 = по часовой
        
        self.rotation_speed = 90 * 360 / 60 * 0.025       
        self.create_menu()
        
        # Привязка событий мыши (только когда лопасти видны)
        self.canvas.bind("<Double-Button-1>", self.toggle_rotation)  # Двойной ЛКМ
        self.canvas.bind("<Double-Button-3>", self.stop_rotation)    # Двойной ПКМ
        
        print("✓ Приложение запущено")
        print("✓ Выберите 'ЛР_Лопасть ► Лопасть' для отображения")
        print(f"✓ Направление вращения: {'ПРОТИВ часовой стрелки' if self.rotation_direction == -1 else 'ПО часовой стрелке'}")
        print(f"✓ Скорость вращения: 45 RPM")

    def create_menu(self):
        # Создаем главное меню
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        # Создаем меню "ЛР_Лопасть"
        lab_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="ЛР_Лопасть", menu=lab_menu)
        
        # Добавляем пункт "Лопасть" в меню
        lab_menu.add_command(label="Лопасть", command=self.show_blades)

    def show_blades(self):
        """Показывает лопасть после выбора пункта меню"""
        if not self.blades_visible:
            self.blades_visible = True
            self.draw_blades()
            print("✓ Лопасть отображена")
            print("✓ Двойной ЛКМ - начать/остановить вращение")
            print("✓ Двойной ПКМ - остановить вращение")
            print(f"✓ Направление: {'ПРОТИВ часовой стрелки' if self.rotation_direction == -1 else 'ПО часовой стрелке'}")
            print(f"✓ Скорость вращения: 45 RPM")


    def draw_blades(self):
        self.canvas.delete("all")
        
        # Центр вращения
        center_x, center_y = 400, 300
        
        # Преобразуем угол в радианы
        angle_rad = math.radians(self.angle)
        
        # Создаем 4 лопасти с разными углами и цветами
        blade_angles = [0, 120, 240]  # Углы для 4 лопастей
        blade_colors = ["blue", "red", "blue", "red"]  # Цвета лопастей
        
        for i, blade_angle in enumerate(blade_angles):
            # Вычисляем угол для текущей лопасти
            current_angle_rad = angle_rad + math.radians(blade_angle)
            
            # Основание лопасти (узкое)
            base_left_x = center_x - self.base_width * math.cos(current_angle_rad)
            base_left_y = center_y - self.base_width * math.sin(current_angle_rad)
            
            base_right_x = center_x + self.base_width * math.cos(current_angle_rad)
            base_right_y = center_y + self.base_width * math.sin(current_angle_rad)
            
            # Конец лопасти (широкий)
            tip_x = center_x + self.h * math.sin(current_angle_rad)
            tip_y = center_y - self.h * math.cos(current_angle_rad)
            
            tip_left_x = tip_x - self.tip_width * math.cos(current_angle_rad)
            tip_left_y = tip_y - self.tip_width * math.sin(current_angle_rad)
            
            tip_right_x = tip_x + self.tip_width * math.cos(current_angle_rad)
            tip_right_y = tip_y + self.tip_width * math.sin(current_angle_rad)
            
            # Рисуем лопасть (трапециевидная форма)
            self.canvas.create_polygon(
                base_left_x, base_left_y,
                base_right_x, base_right_y,
                tip_right_x, tip_right_y,
                tip_left_x, tip_left_y,
                fill=blade_colors[i], outline="black", width=1
            )
        
        
               
        origin_r = 4  # радиус точки
        self.canvas.create_oval(
            -origin_r, -origin_r, origin_r, origin_r,  
            fill="black", outline="black"
        )
      

        # Рисуем зеленый круг в центре радиусом 20
        self.canvas.create_oval(
            center_x - 20, center_y - 20,
            center_x + 20, center_y + 20,
            fill="green", outline="green"
        )
        
       
        self.canvas.create_oval(
            center_x - 3, center_y - 3,
            center_x + 3, center_y + 3,
            fill="black", outline="black"
        )

    def toggle_rotation(self, event):
        if self.blades_visible:  # Только если лопасти видны
            self.rotating = not self.rotating
            status = "включено" if self.rotating else "выключено"
            print(f"Вращение: {status}")
            if self.rotating:
                self.rotate()

    def stop_rotation(self, event):
        if self.blades_visible:  # Только если лопасти видны
            self.rotating = False
            print("Вращение остановлено")

    def rotate(self):
        if self.rotating and self.blades_visible:
            # ★ ВРАЩЕНИЕ С УЧЕТОМ НАПРАВЛЕНИЯ И СКОРОСТИ ★
            self.angle += self.rotation_speed * self.rotation_direction
            
            if self.angle >= 360:
                self.angle -= 360
            elif self.angle < 0:
                self.angle += 360
                
            self.draw_blades()
            self.root.after(25, self.rotate)  # Обновление каждые 25 мс

if __name__ == "__main__":
    try:
        print("Запуск программы Лопасть...")
        root = tk.Tk()
        app = BladeApp(root)
        print("✓ Окно создано успешно!")
        print("✓ Окно пустое - выберите пункт меню 'ЛР_Лопасть ► Лопасть'")
        print("★ ДЛЯ СМЕНЫ НАПРАВЛЕНИЯ ИЗМЕНИТЕ ЗНАЧЕНИЕ rotation_direction В КОДЕ ★")
        print("★ ТЕКУЩАЯ СКОРОСТЬ: 45 RPM ★")
        root.mainloop()
        print("Программа завершена.")
    except Exception as e:
        print(f"Ошибка: {e}")
        input("Нажмите Enter для выхода...")
