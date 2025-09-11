#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"

// Угол поворота фигуры
float global_angle = 0;

// Координаты последнего пикселя, который выбрал пользователь
struct
{
	int X, Y;
} global_clicked_pixel = {-1, -1};

class Painter
{
public:
	void Draw(Frame &frame)
	{
		// Шахматная текстура
		for (int y = 0; y < frame.height; y++)
			for (int x = 0; x < frame.width; x++)
			{
				if ((x + y) % 2 == 0)
					frame.SetPixel(x, y, {230, 255, 230}); // Золотистый цвет
				// frame.SetPixel(x, y, { 217, 168, 14 });
				else
					frame.SetPixel(x, y, {200, 200, 200}); // Чёрный цвет
														   // frame.SetPixel(x, y, { 255, 255, 255 }); // Белый цвет
			}

		int W = frame.width, H = frame.height;
		// Размер рисунка возьмём меньше (7 / 8), чтобы он не касался границ экрана
		float a = 7.0f / 8 * ((W < H) ? W - 1 : H - 1) / sqrt(2);
		if (a < 1)
			return;					// Если окно очень маленькое, то ничего не рисуем
		float angle = global_angle; // Угол поворота
		a = a / 2;

		// радиус окружности
		float R = int(a * sqrt(2) + 0.5f);
		// длина стороны вписанного треугольника
		float a_t = 3 * R / sqrt(3);
		// радиус вписаной в треуголтник окружности
		float r_t = a_t * sqrt(3) / 6;

		// Инициализируем исходные координаты центра и вершин треугольника
		struct
		{
			float x;
			float y;
		} C = {(float)(W / 2), (float)(H / 2)}, A[3] = {{C.x - a_t / 2, C.y + r_t}, {C.x, C.y - R}, {C.x + a_t / 2, C.y + r_t}};

		// Рисуем стороны треугольника
		for (int i = 0; i < 3; i++)
		{
			int i2 = (i + 1) % 3;
			frame.DrawLine( // Добавляем везде 0.5f, чтобы вещественные числа правильно округлялись при преобразовании к целому типу
				int(A[i].x + 0.5f),
				int(A[i].y + 0.5f),
				int(A[i2].x + 0.5f),
				int(A[i2].y + 0.5f), COLOR(0, 0, 255));
		}

		// Инициализируем исходные координаты вершин звезды
		float r_star = r_t / 5;
		struct
		{
			float x;
			float y;
		} B[8] = {{C.x - r_t, C.y}, {C.x - r_star, C.y + r_star}, {C.x, C.y + r_t}, {C.x + r_star, C.y + r_star}, {C.x + r_t, C.y}, {C.x + r_star, C.y - r_star}, {C.x, C.y - r_t}, {C.x - r_star, C.y - r_star}};

		// Поворачиваем все вершины звезды вокруг точки C на угол angle
		for (int i = 0; i < 8; i++)
		{
			float xi = B[i].x, yi = B[i].y;
			B[i].x = (xi - C.x) * cos(-angle) - (yi - C.y) * sin(-angle) + C.x;
			B[i].y = (xi - C.x) * sin(-angle) + (yi - C.y) * cos(-angle) + C.y;
		}

		// Рисуем стороны звезды
		for (int i = 0; i < 8; i++)
		{
			int i2 = (i + 1) % 8;
			frame.DrawLine( // Добавляем везде 0.5f, чтобы вещественные числа правильно округлялись при преобразовании к целому типу
				int(B[i].x + 0.5f),
				int(B[i].y + 0.5f),
				int(B[i2].x + 0.5f),
				int(B[i2].y + 0.5f), COLOR(0, 128, 0));
		}

		// длина стороны вписанного треугольника
		float a_t_2 = 3 * r_t / sqrt(3);
		// радиус вписаной в треуголтник окружности
		float r_t_2 = a_t_2 * sqrt(3) / 6;

		// Инициализируем исходные координаты вершин треугольника2
		struct
		{
			float x;
			float y;
		} D[3] = {{C.x - a_t_2 / 2, C.y + r_t_2}, {C.x, C.y - r_t}, {C.x + a_t_2 / 2, C.y + r_t_2}};

		// Поворачиваем все вершины треугольника вокруг точки C на угол angle
		for (int i = 0; i < 3; i++)
		{
			float xi = D[i].x, yi = D[i].y;
			D[i].x = (xi - C.x) * cos(angle) - (yi - C.y) * sin(angle) + C.x;
			D[i].y = (xi - C.x) * sin(angle) + (yi - C.y) * cos(angle) + C.y;
		}

		// Рисуем стороны треугольника2
		for (int i = 0; i < 3; i++)
		{
			int i2 = (i + 1) % 3;
			frame.DrawLine( // Добавляем везде 0.5f, чтобы вещественные числа правильно округлялись при преобразовании к целому типу
				int(D[i].x + 0.5f),
				int(D[i].y + 0.5f),
				int(D[i2].x + 0.5f),
				int(D[i2].y + 0.5f), COLOR(255, 0, 0));
		}

		// Рисуем описанную окружность
		frame.Circle((int)C.x, (int)C.y, int(a * sqrt(2) + 0.5f), COLOR(0, 0, 0));

		frame.Circle((int)C.x, (int)C.y, r_t + 0.5f, COLOR(0, 0, 0));

		// Рисуем пиксель, на который кликнул пользователь
		if (global_clicked_pixel.X >= 0 && global_clicked_pixel.X < W &&
			global_clicked_pixel.Y >= 0 && global_clicked_pixel.Y < H)
			frame.SetPixel(global_clicked_pixel.X, global_clicked_pixel.Y, {34, 175, 60}); // Пиксель зелёного цвета
	}
};

#endif // PAINTER_H