import tkinter as tk
from tkinter import simpledialog
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from mpl_toolkits.mplot3d.art3d import Poly3DCollection


class Pyramid3D:
    def __init__(self):
        # координаты усечённой пирамиды
        self.bottom = np.array([
            [-2, -2, 0],  # A: нижняя левая задняя
            [2, -2, 0],   # B: нижняя правая задняя
            [2,  2, 0],   # C: нижняя правая передняя
            [-2, -2, 0]   # повтор A для замыкания
        ])
        self.top = np.array([
            [-1, -1, 2],  # A1: верхняя левая задняя
            [1, -1, 2],   # B1: верхняя правая задняя
            [1,  1, 2],   # C1: верхняя правая передняя
            [-1, -1, 2]   # повтор A1
        ])

    def faces(self):
        b, t = self.bottom, self.top
        return [
            [b[0], b[1], b[2], b[3]],  # нижняя грань
            [t[0], t[1], t[2], t[3]],  # верхняя грань
            [b[0], b[1], t[1], t[0]],  # задняя
            [b[1], b[2], t[2], t[1]],  # правая
            [b[2], b[3], t[3], t[2]],  # передняя
            [b[3], b[0], t[0], t[3]]   # левая
        ]

    def is_face_visible(self, face, camera_pos):
        """Определяет, видима ли грань с позиции камеры"""
        if len(face) < 3:
            return False

        v1 = face[1] - face[0]
        v2 = face[2] - face[0]
        normal = np.cross(v1, v2)

        center = np.mean(face, axis=0)
        view_vector = camera_pos - center

        if np.linalg.norm(normal) > 0:
            normal = normal / np.linalg.norm(normal)
        if np.linalg.norm(view_vector) > 0:
            view_vector = view_vector / np.linalg.norm(view_vector)

        return np.dot(normal, view_vector) > 0


class PyramidApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Лабораторная работа №7 — Усечённая пирамида")

        # Параметры камеры
        self.default_theta = 315
        self.default_phi = 45
        self.theta = self.default_theta
        self.phi = self.default_phi

        self.pyramid = Pyramid3D()
        self.mode_hidden = False  # "Удаление невидимых граней"

        # Настройка matplotlib
        self.fig = plt.Figure(figsize=(6, 6))
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.canvas = FigureCanvasTkAgg(self.fig, master=root)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        # Меню
        menubar = tk.Menu(root)
        root.config(menu=menubar)
        menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Пирамида", menu=menu)
        menu.add_command(label="С удалением невидимых граней", command=self.show_with_hidden)
        menu.add_command(label="Без удаления невидимых граней", command=self.show_all)
        menu.add_separator()
        menu.add_command(label="Изменить положение камеры", command=self.change_camera)
        menu.add_command(label="Положение камеры по умолчанию", command=self.reset_camera)
        menu.add_separator()
        menu.add_command(label="Выход", command=root.quit)

        self.draw()

    def get_camera_position(self):
        """Вычисляет позицию камеры на основе углов theta и phi"""
        theta_rad = np.radians(self.theta)
        phi_rad = np.radians(self.phi)
        r = 10

        x = r * np.sin(phi_rad) * np.cos(theta_rad)
        y = r * np.sin(phi_rad) * np.sin(theta_rad)
        z = r * np.cos(phi_rad)

        return np.array([x, y, z])

    def draw_camera_angles(self, camera_pos):
        """Рисуем дуги и прямые для углов θ и φ, а также луч и точку камеры"""
        theta_rad = np.radians(self.theta)
        phi_rad = np.radians(self.phi)

        # единичный вектор направления на камеру
        cam_dir = camera_pos / np.linalg.norm(camera_pos)

        # ---- луч до камеры и точка камеры ----
        cam_len = 3.0
        cam_point = cam_dir * cam_len
        # луч (штриховой)
        self.ax.plot(
            [0, cam_point[0]],
            [0, cam_point[1]],
            [0, cam_point[2]],
            'k--', linewidth=1
        )
        # точка камеры
        self.ax.scatter([cam_point[0]], [cam_point[1]], [cam_point[2]],
                        color='black', s=30)

        # ---- подготовка направлений ----
        proj = cam_dir.copy()
        proj[2] = 0.0
        proj_norm = np.linalg.norm(proj)
        if proj_norm < 1e-6:
            # если камера почти на оси Z — θ не определён, ничего дальше не рисуем
            return

        xy_dir = proj / proj_norm
        z_axis = np.array([0.0, 0.0, 1.0])

        # ---------- угол θ (плоскость XY) ----------
        r_theta = 1.5

        # прямая вдоль оси X
        self.ax.plot([0, r_theta], [0, 0], [0, 0], 'k-', linewidth=1)

        # прямая вдоль проекции камеры на XY
        end_theta = r_theta * np.array([np.cos(theta_rad),
                                        np.sin(theta_rad),
                                        0.0])
        self.ax.plot([0, end_theta[0]],
                     [0, end_theta[1]],
                     [0, end_theta[2]],
                     'k-', linewidth=1)

        # дуга θ
        t_vals = np.linspace(0, theta_rad, 60)
        x_theta = r_theta * np.cos(t_vals)
        y_theta = r_theta * np.sin(t_vals)
        z_theta = np.zeros_like(t_vals)
        self.ax.plot(x_theta, y_theta, z_theta, 'k-', linewidth=1)

        mid_t = theta_rad / 2
        self.ax.text(r_theta * 1.1 * np.cos(mid_t),
                     r_theta * 1.1 * np.sin(mid_t),
                     0,
                     r"$\theta$", color='k')

        # ---------- угол φ (между осью Z и направлением на камеру) ----------
        r_phi = 1.5

        # прямая вдоль оси Z
        self.ax.plot([0, 0], [0, 0], [0, r_phi], 'k-', linewidth=1)

        # прямая к направлению камеры (в плоскости ось Z – проекция)
        end_phi = r_phi * (np.cos(phi_rad) * z_axis + np.sin(phi_rad) * xy_dir)
        self.ax.plot([0, end_phi[0]],
                     [0, end_phi[1]],
                     [0, end_phi[2]],
                     'k-', linewidth=1)

        # дуга φ
        s_vals = np.linspace(0, phi_rad, 60)
        pts = []
        for s in s_vals:
            p = r_phi * (np.cos(s) * z_axis + np.sin(s) * xy_dir)
            pts.append(p)
        pts = np.array(pts)
        self.ax.plot(pts[:, 0], pts[:, 1], pts[:, 2], 'k-', linewidth=1)

        mid_s = phi_rad / 2
        mid_p = r_phi * (np.cos(mid_s) * z_axis + np.sin(mid_s) * xy_dir)
        self.ax.text(mid_p[0] * 1.05,
                     mid_p[1] * 1.05,
                     mid_p[2] * 1.05,
                     r"$\varphi$", color='k')

    def draw(self):
        """Отрисовка пирамиды"""
        self.ax.clear()
        mode_text = "С удалением невидимых граней" if self.mode_hidden else "Все грани"
        self.ax.set_title(f"Камера: θ={self.theta}°, φ={self.phi}°\nРежим: {mode_text}")
        self.ax.set_box_aspect([1, 1, 1])
        self.ax.view_init(elev=self.phi, azim=self.theta)

        faces = self.pyramid.faces()
        colors = [
            'blue',    # низ
            'red',     # верх
            'green',   # задняя
            'orange',  # правая
            'purple',  # передняя
            'brown'    # левая
        ]

        camera_pos = self.get_camera_position()

        if self.mode_hidden:
            visible_faces = []
            visible_colors = []
            for i, face in enumerate(faces):
                if self.pyramid.is_face_visible(face, camera_pos):
                    visible_faces.append(face)
                    visible_colors.append(colors[i])
            for i, face in enumerate(visible_faces):
                poly = Poly3DCollection([face], color=visible_colors[i],
                                        edgecolor='black', alpha=1.0, linewidth=2)
                self.ax.add_collection3d(poly)
        else:
            for i, face in enumerate(faces):
                poly = Poly3DCollection([face], color=colors[i],
                                        edgecolor='black', alpha=0.6, linewidth=1)
                self.ax.add_collection3d(poly)

        # рисуем дуги, прямые и точку камеры
        self.draw_camera_angles(camera_pos)

        # Настройка осей
        self.ax.set_xlim(-3, 3)
        self.ax.set_ylim(-3, 3)
        self.ax.set_zlim(0, 3)
        self.ax.set_xlabel('X')
        self.ax.set_ylabel('Y')
        self.ax.set_zlabel('Z')

        self.ax.xaxis.pane.fill = False
        self.ax.yaxis.pane.fill = False
        self.ax.zaxis.pane.fill = False
        self.ax.grid(True, alpha=0.3)

        self.canvas.draw()

    def show_all(self):
        """Показать все грани"""
        self.mode_hidden = False
        self.draw()

    def show_with_hidden(self):
        """Удалить невидимые грани"""
        self.mode_hidden = True
        self.draw()

    def change_camera(self):
        """Изменить положение камеры"""
        theta = simpledialog.askfloat("Положение камеры", "Введите азимут (θ, градусы):", initialvalue=self.theta)
        phi = simpledialog.askfloat("Положение камеры", "Введите угол подъёма (φ, градусы):", initialvalue=self.phi)
        if theta is not None:
            self.theta = theta
        if phi is not None:
            self.phi = phi
        self.draw()

    def reset_camera(self):
        """Сбросить положение камеры"""
        self.theta = self.default_theta
        self.phi = self.default_phi
        self.draw()


if __name__ == "__main__":
    root = tk.Tk()
    app = PyramidApp(root)
    root.mainloop()
