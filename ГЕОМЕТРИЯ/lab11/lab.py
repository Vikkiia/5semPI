import numpy as np
import matplotlib.pyplot as plt
from typing import List, Tuple, Optional
import os

class LagrangeInterpolator:
    """Класс для работы с интерполяционным полиномом Лагранжа"""
    
    @staticmethod
    def lagrange_value(x_points: np.ndarray, y_points: np.ndarray, x: float) -> float:
        """
        Вычисление значения интерполяционного полинома Лагранжа в точке x
        
        Args:
            x_points: массив значений x узловых точек
            y_points: массив значений y узловых точек
            x: точка, в которой вычисляется значение
            
        Returns:
            Значение полинома в точке x
        """
        if len(x_points) != len(y_points):
            raise ValueError("Размеры массивов x_points и y_points должны совпадать")
        
        n = len(x_points)
        result = 0.0
        
        for i in range(n):
            term = y_points[i]
            for j in range(n):
                if i != j:
                    term *= (x - x_points[j]) / (x_points[i] - x_points[j])
            result += term
        
        return result
    
    @staticmethod
    def original_function(x: float) -> float:
        """Исходная функция: y = (2 + cos(x))^sin(2x)"""
        return (2 + np.cos(x)) ** np.sin(2 * x)
    
    @staticmethod
    def generate_points(x_l: float = 0, x_h: float = np.pi, dx: float = np.pi/4) -> Tuple[np.ndarray, np.ndarray]:
        """
        Генерация точек исходной функции
        
        Args:
            x_l: начало отрезка
            x_h: конец отрезка
            dx: шаг
            
        Returns:
            Кортеж (X, Y) с точками
        """
        n_points = int((x_h - x_l) / dx) + 1
        X = np.array([x_l + i * dx for i in range(n_points)])
        Y = np.array([LagrangeInterpolator.original_function(x) for x in X])
        return X, Y
    
    @staticmethod
    def interpolate(x_points: np.ndarray, y_points: np.ndarray, 
                    x_start: float = 0, x_end: float = np.pi, step: float = 0.2) -> Tuple[np.ndarray, np.ndarray]:
        """
        Интерполяция полиномом Лагранжа
        
        Args:
            x_points: узлы интерполяции по x
            y_points: значения функции в узлах
            x_start: начало интервала интерполяции
            x_end: конец интервала интерполяции
            step: шаг интерполяции
            
        Returns:
            Кортеж (X_interp, Y_interp) с интерполированными точками
        """
        n_interp = int((x_end - x_start) / step)
        X_interp = np.array([x_start + i * step for i in range(n_interp + 1)])
        Y_interp = np.array([LagrangeInterpolator.lagrange_value(x_points, y_points, x) 
                           for x in X_interp])
        return X_interp, Y_interp
    
    @staticmethod
    def save_to_file(filename: str, X: np.ndarray, Y: np.ndarray, description: str = ""):
        """Сохранение данных в файл формата Mathcad"""
        with open(filename, 'w') as f:
            if description:
                f.write(f"# {description}\n")
            f.write(f"X := {X.tolist()}\n")
            f.write(f"Y := {Y.tolist()}\n")
    
    @staticmethod
    def run_lagrange():
        """Выполнение задания по полиному Лагранжа"""
        print("\n" + "="*60)
        print("ПОЛИНОМ ЛАГРАНЖА")
        print("="*60)
        
        # 1. Генерация исходных точек
        X_points, Y_points = LagrangeInterpolator.generate_points()
        print(f"Узлы интерполяции (n={len(X_points)}):")
        for i, (x, y) in enumerate(zip(X_points, Y_points)):
            print(f"  X[{i}] = {x:.4f}, Y[{i}] = {y:.4f}")
        
        # Сохранение в файл
        LagrangeInterpolator.save_to_file("lagrange_points.txt", X_points, Y_points, 
                                         "Точки для интерполяции полиномом Лагранжа")
        
        # 2. Интерполяция с шагом 0.2
        X_interp, Y_interp = LagrangeInterpolator.interpolate(X_points, Y_points, step=0.2)
        print(f"\nИнтерполяция на отрезке [0, π] с шагом 0.2:")
        print(f"  Количество точек: {len(X_interp)}")
        print(f"  Первые 5 точек интерполяции:")
        for i in range(min(5, len(X_interp))):
            print(f"    X_interp[{i}] = {X_interp[i]:.4f}, Y_interp[{i}] = {Y_interp[i]:.4f}")
        
        # Сохранение интерполированных точек
        LagrangeInterpolator.save_to_file("lagrange_interpolated.txt", X_interp, Y_interp,
                                         "Интерполированные значения полинома Лагранжа")
        
        # 3. Построение графика
        print("\nПостроение графика...")
        LagrangeInterpolator.plot_results(X_points, Y_points, X_interp, Y_interp)
        
        return X_points, Y_points, X_interp, Y_interp
    
    @staticmethod
    def plot_results(X_points: np.ndarray, Y_points: np.ndarray, 
                     X_interp: np.ndarray, Y_interp: np.ndarray):
        """Построение графиков"""
        # Генерация точек для исходной функции
        x_dense = np.linspace(0, np.pi, 200)
        y_dense = np.array([LagrangeInterpolator.original_function(x) for x in x_dense])
        
        # Создание графика
        plt.figure(figsize=(12, 8))
        
        # Исходная функция
        plt.plot(x_dense, y_dense, 'b-', linewidth=2, label='Исходная функция: (2+cos(x))^{sin(2x)}')
        
        # Узлы интерполяции
        plt.plot(X_points, Y_points, 'ro', markersize=10, markerfacecolor='red', 
                markeredgecolor='black', label='Узлы интерполяции')
        
        # Полином Лагранжа
        plt.plot(X_interp, Y_interp, 'g--', linewidth=3, label='Полином Лагранжа')
        
        # Настройки графика
        plt.title('Интерполяция полиномом Лагранжа', fontsize=16, fontweight='bold')
        plt.xlabel('x', fontsize=12)
        plt.ylabel('y', fontsize=12)
        plt.legend(fontsize=11, loc='best')
        plt.grid(True, alpha=0.3)
        plt.xlim(-0.1, np.pi + 0.1)
        
        # Добавление сетки и оформления
        ax = plt.gca()
        ax.spines['right'].set_visible(False)
        ax.spines['top'].set_visible(False)
        
        # Добавление меток на оси
        ax.set_xticks([0, np.pi/4, np.pi/2, 3*np.pi/4, np.pi])
        ax.set_xticklabels(['0', 'π/4', 'π/2', '3π/4', 'π'])
        
        plt.tight_layout()
        plt.savefig('lagrange_plot.png', dpi=150, bbox_inches='tight')
        print("График сохранен как 'lagrange_plot.png'")
        plt.show()


class BezierCurve:
    """Класс для работы с кривыми Безье"""
    
    @staticmethod
    def calculate_curve(control_points: List[Tuple[float, float]], 
                       num_segments: int = 100) -> np.ndarray:
        """
        Вычисление координат кривой Безье по геометрическому алгоритму де Кастельжо
        
        Args:
            control_points: список контрольных точек [(x1, y1), (x2, y2), ...]
            num_segments: число отрезков для параметра t
            
        Returns:
            Массив точек кривой Безье
        """
        n = len(control_points)
        if n < 2:
            raise ValueError("Необходимо как минимум 2 контрольные точки")
        
        curve_points = []
        
        # Преобразуем контрольные точки в numpy массив
        points = np.array(control_points, dtype=float)
        
        for t in np.linspace(0, 1, num_segments):
            # Копируем точки для алгоритма де Кастельжо
            temp_points = points.copy()
            
            # Геометрический алгоритм де Кастельжо
            for k in range(1, n):
                for i in range(n - k):
                    temp_points[i] = (1 - t) * temp_points[i] + t * temp_points[i + 1]
            
            curve_points.append(temp_points[0])
        
        return np.array(curve_points)
    
    @staticmethod
    def get_example_curves() -> dict:
        """Возвращает примеры кривых Безье из учебника (рис. 14.5)"""
        return {
            'Пример 1': [
                (0, 0),
                (np.pi/3, np.pi/3),
                (4*np.pi/3, np.pi/3),
                (5*np.pi/3, 0)
            ],
            'Пример 2': [
                (0, 0),
                (1.5*np.pi/2, 2),
                (3*np.pi/2, 0)
            ],
            'Пример 3': [
                (0, np.sin(0)),
                (np.pi/2, np.sin(np.pi/2)),
                (np.pi, np.sin(np.pi)),
                (3*np.pi/2, np.sin(3*np.pi/2)),
                (2*np.pi, np.sin(2*np.pi))
            ],
            'Пример 4': [
                (2*np.pi/4, 0),
                (7*np.pi/4, 3),
                (0, 3),
                (5*np.pi/4, 0)
            ]
        }
    
    @staticmethod
    def save_bezier_data(filename: str, control_points: List[Tuple[float, float]], 
                         curve_points: np.ndarray, description: str = ""):
        """Сохранение данных кривой Безье в файл"""
        with open(filename, 'w') as f:
            if description:
                f.write(f"# {description}\n\n")
            
            # Сохраняем контрольные точки
            f.write("# Контрольные точки (опорная ломаная)\n")
            control_array = np.array(control_points)
            f.write(f"X_control := {control_array[:, 0].tolist()}\n")
            f.write(f"Y_control := {control_array[:, 1].tolist()}\n\n")
            
            # Сохраняем точки кривой Безье
            f.write("# Точки кривой Безье\n")
            f.write(f"X_curve := {curve_points[:, 0].tolist()}\n")
            f.write(f"Y_curve := {curve_points[:, 1].tolist()}\n")
    
    @staticmethod
    def run_bezier_examples():
        """Выполнение задания по кривым Безье"""
        print("\n" + "="*60)
        print("КРИВЫЕ БЕЗЬЕ")
        print("="*60)
        
        examples = BezierCurve.get_example_curves()
        
        # Создаем сетку графиков
        fig, axes = plt.subplots(2, 2, figsize=(14, 10))
        axes = axes.flatten()
        
        print("Построение 4 примеров кривых Безье:")
        
        for idx, (title, points) in enumerate(examples.items()):
            ax = axes[idx]
            
            # Преобразуем точки в массив
            points_array = np.array(points)
            
            # Вычисляем кривую Безье
            curve = BezierCurve.calculate_curve(points, 200)
            
            # Рисуем контрольные точки и ломаную
            ax.plot(points_array[:, 0], points_array[:, 1], 'ro-', 
                   linewidth=1.5, markersize=6, markerfacecolor='red',
                   markeredgecolor='black', label='Опорная ломаная')
            
            # Рисуем кривую Безье
            ax.plot(curve[:, 0], curve[:, 1], 'b-', 
                   linewidth=2.5, label='Кривая Безье')
            
            # Настройки графика
            ax.set_title(title, fontsize=14, fontweight='bold')
            ax.set_xlabel('x', fontsize=10)
            ax.set_ylabel('y', fontsize=10)
            ax.legend(fontsize=9)
            ax.grid(True, alpha=0.3)
            
            # Сохранение данных в файл
            BezierCurve.save_bezier_data(f'bezier_example_{idx+1}.txt', 
                                        points, curve, title)
            
            print(f"  {title}: сохранен в файл 'bezier_example_{idx+1}.txt'")
            print(f"    Контрольных точек: {len(points)}")
            print(f"    Точек на кривой: {len(curve)}")
        
        plt.tight_layout()
        plt.savefig('bezier_examples.png', dpi=150, bbox_inches='tight')
        print("\nВсе графики сохранены в файл 'bezier_examples.png'")
        plt.show()


class Menu:
    """Имитация меню для выбора задачи"""
    
    @staticmethod
    def display():
        """Отображение меню"""
        while True:
            print("\n" + "="*60)
            print("ЛАБОРАТОРНАЯ РАБОТА №11: ПОСТРОЕНИЕ КРИВЫХ НА ПЛОСКОСТИ")
            print("="*60)
            print("1. Построение интерполяционного полинома Лагранжа")
            print("2. Построение кривых Безье (4 примера)")
            print("3. Выполнить все задания")
            print("4. Выход")
            print("-"*60)
            
            choice = input("Выберите пункт меню (1-4): ").strip()
            
            if choice == '1':
                LagrangeInterpolator.run_lagrange()
            elif choice == '2':
                BezierCurve.run_bezier_examples()
            elif choice == '3':
                print("\n" + "="*60)
                print("ВЫПОЛНЕНИЕ ВСЕХ ЗАДАНИЙ")
                print("="*60)
                LagrangeInterpolator.run_lagrange()
                BezierCurve.run_bezier_examples()
            elif choice == '4':
                print("\nВыход из программы.")
                break
            else:
                print("\nОшибка: выберите пункт от 1 до 4")
            
            input("\nНажмите Enter для продолжения...")


def main():
    """Основная функция программы"""
    print("="*70)
    print("Лабораторная работа №11: Построение кривых на плоскости")
    print("="*70)
    print("Тема: Построение интерполяционного полинома Лагранжа и кривых Безье")
    print()
    print("Задание выполнено на Python с использованием библиотек:")
    print("  - NumPy для вычислений")
    print("  - Matplotlib для визуализации")
    print("="*70)
    
    # Создаем необходимые директории
    os.makedirs("output", exist_ok=True)
    
    # Запускаем меню
    Menu.display()


if __name__ == "__main__":
    # Проверка зависимостей
    try:
        import numpy as np
        import matplotlib.pyplot as plt
    except ImportError as e:
        print(f"Ошибка: Не установлены необходимые библиотеки.")
        print(f"Установите их с помощью: pip install numpy matplotlib")
        exit(1)
    
    # Запуск программы
    main()