# Математические основы перспективной проекции в 3D-графике

## Общая схема преобразований

Мировые координаты → Пространство камеры → Clip Space → NDC → Экранные координаты
(World) (View) (Clip) (NDC) (Screen)


## 1. Матрица вида (View Matrix) - LookAt

### Цель
Переместить камеру в начало координат и повернуть мир так, чтобы камера смотрела вдоль оси -Z.

### Математический вывод

**Шаг 1: Вычисление базисных векторов камеры**


forward = normalize(center - eye) // направление взгляда
right = normalize(cross(forward, up)) // правосторонний вектор
trueUp = cross(right, forward) // перпендикулярный вектор вверх


**Почему `-forward` в матрице?**
В OpenGL камера смотрит вдоль **-Z**, поэтому:
- `right` → ось X
- `trueUp` → ось Y
- `-forward` → ось Z

**Шаг 2: Построение матрицы**

Матрица вида = Поворот × Перенос:

[ R_x U_x -F_x 0 ] [ 1 0 0 -eye.x ]
[ R_y U_y -F_y 0 ] × [ 0 1 0 -eye.y ]
[ R_z U_z -F_z 0 ] [ 0 0 1 -eye.z ]
[ 0 0 0 1 ] [ 0 0 0 1 ]


Где:
- R = right (нормализованный)
- U = trueUp (нормализованный)
- F = forward (нормализованный)

**Результат в коде:**
```cpp
viewMatrix[0][3] = -dot(right, eye)   // перенос по X
viewMatrix[1][3] = -dot(trueUp, eye)  // перенос по Y
viewMatrix[2][3] = dot(forward, eye)  // перенос по Z (знак + из-за -forward)


Что означают функции:
Функция	Что делает	Формула
dot(a,b)	Скалярное произведение	a·b = a.x*b.x + a.y*b.y + a.z*b.z
cross(a,b)	Векторное произведение	a × b = (a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x)
normalize(v)	Нормализация (длина = 1)	v / |v| = v / √(x² + y² + z²)
length(v)	Длина вектора	√(x² + y² + z²)
2. Матрица перспективной проекции
Цель
Создать эффект "дальше-меньше" и преобразовать пирамиду видимости (frustum) в куб NDC [-1,1]³.

Геометрия пирамиды видимости (Frustum)

                        Near Plane (z = -n)      Far Plane (z = -f)
                    ┌──────────────┐
                    │              │
Камера в (0,0,0) →  │       ●      │           Смотрит вдоль -Z
                    │              │
                    └──────────────┘
                    ←─── aspect ───→

Где:
- n = nearPlane (ближняя плоскость, > 0)
- f = farPlane (дальняя плоскость, > n)
- aspect = screenWidth / screenHeight
- fov = угол обзора в градусах

Исходная формула перспективы
Из подобия треугольников:

x_proj = (n * x) / (-z)
y_proj = (n * y) / (-z)
z_proj = (f * (z + n)) / (n * (f - n))?  // упрощённо

где n = nearPlane, z < 0 (так как смотрим вдоль -Z)


Матрица проекции в Clip Space

[ f/aspect   0         0              0      ] [ x ]
[    0       f         0              0      ] [ y ]
[    0       0    (f+n)/(f-n)    (2fn)/(f-n) ] [ z ]
[    0       0        -1              0      ] [ w ]

где f = 1/tan(fov/2)

Объяснение каждого элемента
Элемент	Формула	Почему?
[0][0]	f / aspect	Масштабирует X с учётом соотношения сторон экрана
[1][1]	f	Масштабирует Y на основе FOV
[2][2]	(f+n)/(f-n)	Нормализует глубину в диапазон [-1,1]
[2][3]	(2fn)/(f-n)	Смещение для правильного отображения глубины
[3][2]	-1	Создаёт w = -z для перспективного деления

Вывод формулы для f
Из тригонометрии:

tan(fov/2) = opposite/adjacent = (height/2) / n

Так как в NDC высота = 2, то:



tan(fov/2) = 1 / n  →  n = 1/tan(fov/2)

Следовательно, f = n = 1/tan(fov/2)



Что происходит с точкой после умножения на матрицу:


x_clip = (f/aspect) * x
y_clip = f * y
z_clip = ((f+n)/(f-n)) * z + (2fn)/(f-n)
w_clip = -z

Перспективное деление
Делим на w_clip для перехода в NDC (Normalized Device Coordinates):

x_ndc = x_clip / w_clip = (f/aspect * x) / (-z)
y_ndc = y_clip / w_clip = (f * y) / (-z)
z_ndc = z_clip / w_clip = ((f+n)/(f-n)) + (2fn)/(f-n)/(-z)

Результат: X и Y в диапазоне [-1, 1], Z в диапазоне [-1, 1] (но нелинейно).


3. Преобразование NDC → Экранные координаты
NDC диапазон
После перспективного деления координаты находятся в диапазоне [-1, 1] для X и Y.

        NDC пространство           Экран (пиксели)
    (-1, 1) ┌─────────┐ (1, 1)     (0, 0) ┌─────────┐ (w, 0)
            │         │                   │         │
            │   ●(0,0)│                   │    ●    │
            │         │                   │         │
    (-1,-1) └─────────┘ (1,-1)     (0, h) └─────────┘ (w, h)


Стандартная формула преобразования:


screenX = (ndcX + 1) * screenWidth / 2
screenY = (1 - ndcY) * screenHeight / 2

Почему 1 - ndcY?

В NDC: Y↑ (вверх = +1)

В экранных координатах: Y↓ (вниз = +1, так как начало в левом верхнем углу)



Альтернативная форма (использована в коде):


screenX = ndcX * screenWidth / 2
screenY = -ndcY * screenHeight / 2

Это эквивалентно, т.к. предполагает, что центр экрана находится в точке (0,0) на уровне NDC.

4. Полный поток данных на примере
Пусть у нас есть:

Точка в мире: P = (2, 3, -5)

Камера в: eye = (0, 0, 0)

Смотрит в: center = (0, 0, -1)

FOV = 90°, aspect = 1, near = 1, far = 10

Экран: 800×600

Этап 1: Преобразование в пространство камеры


forward = normalize(0,0,-1) = (0,0,-1)
right = normalize(cross((0,0,-1), (0,1,0))) = (1,0,0)
trueUp = (0,1,0)

viewMatrix = [
    [1, 0, 0, 0]
    [0, 1, 0, 0]
    [0, 0, 1, 0]
    [0, 0, 0, 1]
]

viewResult = (2, 3, -5, 1)  // так как камера в начале координат

Этап 2: Проекция


f = 1/tan(45°) = 1
aspect = 1
near = 1, far = 10

projMatrix = [
    [1, 0, 0, 0]
    [0, 1, 0, 0]
    [0, 0, (10+1)/(10-1)=11/9, (2*1*10)/(10-1)=20/9]
    [0, 0, -1, 0]
]

clipSpace = (2, 3, -5*(11/9) + 20/9 = (-55+20)/9 = -35/9 ≈ -3.889, 5)
clipSpace = (2, 3, -3.889, 5)

Этап 3: Перспективное деление


ndcX = 2 / 5 = 0.4
ndcY = 3 / 5 = 0.6
ndcZ = -3.889 / 5 = -0.7778

Этап 4: В экранные координаты


screenX = 0.4 * 800 / 2 = 160
screenY = -0.6 * 600 / 2 = -180  // может потребоваться смещение

5. Однородные координаты (4D)
Зачем нужен четвёртый компонент w?
Для точек: w = 1
Для векторов: w = 0

Тип	Представление	Почему?
Точка	(x, y, z, 1)	Должна изменяться при переносе
Вектор	(x, y, z, 0)	Не должен изменяться при переносе
Преимущества:
Объединение поворота и переноса в одной матрице

Представление перспективы (w = -z)

Корректная работа с бесконечно удалёнными точками

6. Ключевые математические концепции
Аффинные преобразования (матрицы 4×4)
Тип	Матрица	Пример
Вращение	Ортогональная	Поворот на угол θ вокруг оси Z
Перенос	Единичная + последний столбец	(1,0,0,tx; 0,1,0,ty; 0,0,1,tz; 0,0,0,1)
Масштаб	Диагональная	(sx,0,0,0; 0,sy,0,0; 0,0,sz,0; 0,0,0,1)
Порядок умножения матриц

Результат = M_projection × M_view × M_model × Вектор

Важно: Матрицы умножаются справа налево!



7. Частые проблемы и их решение
Проблема 1: Точки за камерой (z > 0)

if (clipSpace[3] == 0) return {-1, -1};  // отбрасываем

Проблема 2: Z-fighting (конфликт глубины)
Причина: Нелинейное распределение z-буфера
Решение: Уменьшить соотношение far/near (рекомендуется far/near ≤ 1000)

8. Рекомендуемая литература
Книги:
"Mathematics for 3D Game Programming and Computer Graphics" - Eric Lengyel (Глава 4)

"3D Math Primer for Graphics and Game Development" - Fletcher Dunn, Ian Parberry

"Real-Time Rendering" - Tomas Akenine-Möller, Eric Haines (Глава 4)

"OpenGL Programming Guide" (Red Book) - Appendix G

"Foundations of 3D Computer Graphics" - Steven J. Gortler

Онлайн-ресурсы:
LearnOpenGL - Coordinate Systems (learnopengl.com)

Song Ho Ahn - OpenGL Projection Matrix (songho.ca)

Scratchapixel - The Perspective Projection (scratchapixel.com)

9. Формулы в компактном виде
LookAt матрица:

R_x   U_x   -F_x   -(R·eye)
R_y   U_y   -F_y   -(U·eye)
R_z   U_z   -F_z    (F·eye)
0     0      0       1

Проекционная матрица:


[ 1/(aspect·tan(fov/2))    0               0                    0          ]
[          0           1/tan(fov/2)        0                    0          ]
[          0               0         (f+n)/(f-n)          (2fn)/(f-n)      ]
[          0               0              -1                    0          ]



Источник

    "Mathematics for 3D Game Programming" by Eric Lengyel (Chapter 4)

    "3D Math Primer for Graphics and Game Development" by Dunn & Parberry

Источники:

    "OpenGL Programming Guide" (Red Book) - Appendix G

    "Real-Time Rendering" by Tomas Akenine-Möller (Chapter 4)

Рекомендуемая литература

    "Foundation of 3D Computer Graphics" by Steven J. Gortler - отличное введение

    "3D Math Primer for Graphics and Game Development" - практический подход

    "Learn OpenGL" (learnopengl.com) - глава о координатных пространствах

    "The Perspective Matrix" by Song Ho Ahn (songho.ca) - подробный вывод формул

Онлайн ресурсы для визуализации

    LearnOpenGL - Coordinate Systems (с интерактивными примерами)

    Desmos 3D - для экспериментов с матрицами

    glm (OpenGL Mathematics) - библиотека с реализацией всех этих функций

