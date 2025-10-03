#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"

// Установите 1 для отрисовки основного варианта, 0 - для отрисовки задания с защиты (сектор-круг)
#define MAIN_TASK 1


// Угол поворота фигуры
float global_angle = 0;

// Координаты последнего пикселя, который выбрал пользователь 
struct
{
	int X, Y;
} global_clicked_pixel = { -1, -1 };

enum DrawMode {
	SECTOR = 0,
	RADIAL = 1,
	BARYCENTRIC = 2
};

DrawMode bigCircleDrawMode = SECTOR;
DrawMode triangleDrawMode = BARYCENTRIC;
DrawMode smallCircleDrawMode = RADIAL;
DrawMode starDrawMode = BARYCENTRIC;

typedef struct
{
	float x;
	float y;
} coordinate;

// Класс для расчёта барицентрической интерполяции
class BarycentricInterpolator
{
	float x0, y0, x1, y1, x2, y2, S;
	COLOR C0, C1, C2;

public:
	BarycentricInterpolator(float _x0, float _y0, float _x1, float _y1, float _x2, float _y2, COLOR A0, COLOR A1, COLOR A2) :
		x0(_x0), y0(_y0), x1(_x1), y1(_y1), x2(_x2), y2(_y2),
		S((_y1 - _y2)* (_x0 - _x2) + (_x2 - _x1) * (_y0 - _y2)), C0(A0), C1(A1), C2(A2)
	{
	}


	COLOR color(float x, float y)
	{
		// Барицентрическая интерполяция
		float h0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / S;
		float h1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / S;
		float h2 = 1 - h0 - h1;
		float r = h0 * C0.RED + h1 * C1.RED + h2 * C2.RED;
		float g = h0 * C0.GREEN + h1 * C1.GREEN + h2 * C2.GREEN;
		float b = h0 * C0.BLUE + h1 * C1.BLUE + h2 * C2.BLUE;
		float a = h0 * C0.ALPHA + h1 * C1.ALPHA + h2 * C2.ALPHA;
		// Из-за погрешности аппроксимации треугольника учитываем, что центр закрашиваемого пикселя может находится вне треугольника.
		// По этой причине значения r, g, b могут выйти за пределы диапазона [0, 255].
		return COLOR(r, g, b, a);
	}
};

// Класс для расчёта радиальной интерполяции
class RadialInterpolator
{
	float cx, cy; // Центр прямоугольника
	std::vector<COLOR> colors; // Цвета радиальной заливки
	float angle;  // Начальный угол заливки

public:
	RadialInterpolator(float _x0, float _y0, float _x1, float _y1, COLOR A0, COLOR A1, float _angle) :
		cx((_x0 + _x1) / 2.0f), cy((_y0 + _y1) / 2.0f),
		colors({ A0, A1 }), angle(_angle)
	{
	}
	RadialInterpolator(float _x0, float _y0, float _x1, float _y1, std::vector<COLOR> _colors, float _angle) :
		cx((_x0 + _x1) / 2.0f), cy((_y0 + _y1) / 2.0f),
		colors(_colors), angle(_angle)
	{
	}

	COLOR color(float x, float y)
	{
		double dx = (double)x - cx, dy = (double)y - cy;
		double radius = sqrt(dx * dx + dy * dy);

		double h0 = radius / 40 + angle;
		h0 -= floor(h0);

		h0 *= colors.size();
		int h0IndexColors = h0;
		int h1IndexColors = h0IndexColors + 1;
		COLOR colorh01 = colors[h0IndexColors % colors.size()];
		COLOR colorh11 = colors[h1IndexColors % colors.size()];
		h0 -= floor(h0);
		double h1 = 1 - h0;

		double r = h0 * colorh11.RED + h1 * colorh01.RED;
		double g = h0 * colorh11.GREEN + h1 * colorh01.GREEN;
		double b = h0 * colorh11.BLUE + h1 * colorh01.BLUE;

		return COLOR(r, g, b);

	}
};

// Класс для расчёта барицентрической интерполяции
class SectorInterpolator
{
	float c_x, c_y;
	COLOR C0, C1, C2;


	inline double getangle(int x, int y) {
		double x1 = 0;
		double y1 = 1;
		double x2 = x - c_x;
		double y2 = y - c_y;

		double dot = x1 * x2 + y1 * y2;
		double det = x1 * y2 - y1 * x2;
		return atan2(det, dot);
	}
public:
	SectorInterpolator(float c_x, float c_y) :
		c_x(c_x), c_y(c_y)
	{
	}


	COLOR color(float x, float y)
	{
		return HSVCOLOR(180 + (getangle(x, y) * 180) / 3.14, 1, 1).convertToRgb();
	}
};

class Painter
{
public:

	void Draw(Frame& frame)
	{
		// Шахматная текстура
		for (int y = 0; y < frame.height; y++)
			for (int x = 0; x < frame.width; x++)
			{
				if ((x + y) % 2 == 0)
					frame.SetPixel(x, y, { 230, 255, 230 });	// Золотистый цвет
				//frame.SetPixel(x, y, { 217, 168, 14 });	
				else
					frame.SetPixel(x, y, { 200, 200, 200 }); // Чёрный цвет
				//frame.SetPixel(x, y, { 255, 255, 255 }); // Белый цвет
			}


		int W = frame.width, H = frame.height;
		// Размер рисунка возьмём меньше (7 / 8), чтобы он не касался границ экрана 
		float a = 7.0f / 8 * ((W < H) ? W - 1 : H - 1);
		if (a < 1) return; // Если окно очень маленькое, то ничего не рисуем
		float angle = -global_angle; // Угол поворота
		a = a / 2;
		coordinate C = { W / 2, H / 2 };


		// Код для отрисовки основного задания.
		if (MAIN_TASK) {
			double t = (3 * a) / sqrt(3);
			coordinate triangleA = { C.x, C.y - a };
			coordinate triangleB = { C.x - t / 2, C.y + a / 2 };
			coordinate triangleC = { C.x + t / 2, C.y + a / 2 };

			Matrix S = { 1, 0, 0,
							0, 1, 0,
							0, 0, 1 };
			Matrix R = { cos(angle), -sin(angle),  0,
							sin(angle),  cos(angle),  0,
									0,           0,  1 };
			Matrix T = { 1, 0, W / 2.0,
							0, 1, H / 2.0,
							0, 0,       1 };
			Matrix SRT = (T.multiply(R)).multiply(S);
			double starOffset = a / 12;
			coordinate star[8] = {
				{ 0, a / 2 },
				{ starOffset, starOffset },
				{ a / 2, 0 },
				{ starOffset, -starOffset },
				{ 0, -a / 2 },
				{ -starOffset, -starOffset },
				{ -a / 2, 0 },
				{ -starOffset, starOffset } };

			for (int i = 0; i < 8; i++)
			{
				Vector pointVector = { star[i].x, star[i].y, 1 };
				pointVector = SRT.multiply(pointVector);
				star[i].x = pointVector.vector[0];
				star[i].y = pointVector.vector[1];
			}

			bool starSelected = frame.IsPointInTriangle(
				star[7].x, star[7].y, star[0].x, star[0].y, star[1].x, star[1].y,
				global_clicked_pixel.X, global_clicked_pixel.Y) ||
				frame.IsPointInTriangle(
					star[1].x, star[1].y, star[2].x, star[2].y, star[3].x, star[3].y,
					global_clicked_pixel.X, global_clicked_pixel.Y) ||
				frame.IsPointInTriangle(
					star[5].x, star[5].y, star[4].x, star[4].y, star[3].x, star[3].y,
					global_clicked_pixel.X, global_clicked_pixel.Y) ||
				frame.IsPointInTriangle(
					star[5].x, star[5].y, star[6].x, star[6].y, star[7].x, star[7].y,
					global_clicked_pixel.X, global_clicked_pixel.Y) ||
				frame.IsPointInTriangle(
					star[7].x, star[7].y, star[1].x, star[1].y, star[3].x, star[3].y,
					global_clicked_pixel.X, global_clicked_pixel.Y) ||
				frame.IsPointInTriangle(
					star[7].x, star[7].y, star[5].x, star[5].y, star[3].x, star[3].y,
					global_clicked_pixel.X, global_clicked_pixel.Y);

			bool smallCircleSelected = !starSelected &&
				frame.IsPointInCircle((int)C.x, (int)C.y, (int)(a * 0.5),
					global_clicked_pixel.X, global_clicked_pixel.Y);

			bool triangleSelected = !smallCircleSelected && !starSelected && frame.IsPointInTriangle(
				triangleA.x, triangleA.y,
				triangleB.x, triangleB.y,
				triangleC.x, triangleC.y,
				global_clicked_pixel.X, global_clicked_pixel.Y);

			bool bigCircleSelected = !triangleSelected && !smallCircleSelected && !starSelected &&
				frame.IsPointInCircle((int)C.x, (int)C.y, (int)a,
					global_clicked_pixel.X, global_clicked_pixel.Y);

			float x0 = 0, y0 = 0, x1 = frame.width, y1 = frame.height;


			RadialInterpolator selected(x0, y0, x1, y1, HSVCOLOR(255, 1, 1).convertToRgb(), COLOR(255, 0, 0), 0);

			SectorInterpolator sectorInterpolator(C.x, C.y);

			RadialInterpolator radialInterpolator(x0, y0, x1, y1, COLOR(255, 0, 0), HSVCOLOR(311, .1, .1).convertToRgb(), global_angle);

			BarycentricInterpolator triangleInterpolator(
				triangleA.x + 0.5, triangleA.y + 0.5,
				triangleB.x + 0.5, triangleB.y + 0.5,
				triangleC.x + 0.5, triangleC.y + 0.5,
				COLOR(255, 255, 255, 60),
				HSVCOLOR(51, 1, .5).convertToRgb(),
				COLOR(128, 128, 128));


			// Рисуем описанную окружность

			SectorInterpolator sector(C.x, C.y);
			if (bigCircleSelected)
				frame.Circle((int)C.x, (int)C.y, (int)a, selected);
			else if (bigCircleDrawMode == DrawMode::SECTOR)
				frame.Circle((int)C.x, (int)C.y, (int)a, sectorInterpolator);
			else if (bigCircleDrawMode == DrawMode::RADIAL)
				frame.Circle((int)C.x, (int)C.y, (int)a, radialInterpolator);
			else if (bigCircleDrawMode == DrawMode::BARYCENTRIC)
				frame.Circle((int)C.x, (int)C.y, (int)a, triangleInterpolator);




			//Рисуем треугольник


			if (triangleSelected) {
				frame.Triangle(
					triangleA.x + 0.5, triangleA.y + 0.5,
					triangleB.x + 0.5, triangleB.y + 0.5,
					triangleC.x + 0.5, triangleC.y + 0.5,
					selected);
			}
			else if (triangleDrawMode == DrawMode::SECTOR)
				frame.Triangle(
					triangleA.x + 0.5, triangleA.y + 0.5,
					triangleB.x + 0.5, triangleB.y + 0.5,
					triangleC.x + 0.5, triangleC.y + 0.5,
					sectorInterpolator);
			else if (triangleDrawMode == DrawMode::RADIAL)
				frame.Triangle(
					triangleA.x + 0.5, triangleA.y + 0.5,
					triangleB.x + 0.5, triangleB.y + 0.5,
					triangleC.x + 0.5, triangleC.y + 0.5,
					radialInterpolator);
			else if (triangleDrawMode == DrawMode::BARYCENTRIC)
				frame.Triangle(
					triangleA.x + 0.5, triangleA.y + 0.5,
					triangleB.x + 0.5, triangleB.y + 0.5,
					triangleC.x + 0.5, triangleC.y + 0.5,
					triangleInterpolator);


			if (smallCircleSelected)
				frame.Circle((int)C.x, (int)C.y, (int)(a * 0.5), selected);
			else if (smallCircleDrawMode == DrawMode::SECTOR)
				frame.Circle((int)C.x, (int)C.y, (int)(a * 0.5), sectorInterpolator);
			else if (smallCircleDrawMode == DrawMode::RADIAL)
				frame.Circle((int)C.x, (int)C.y, (int)(a * 0.5), radialInterpolator);
			else if (smallCircleDrawMode == DrawMode::BARYCENTRIC)
				frame.Circle((int)C.x, (int)C.y, (int)(a * 0.5), triangleInterpolator);


			// Добавим заливку для звезды в центре
			if (starSelected) {
				frame.Triangle(star[7].x, star[7].y, star[0].x, star[0].y, star[1].x, star[1].y, selected);
				frame.Triangle(star[1].x, star[1].y, star[2].x, star[2].y, star[3].x, star[3].y, selected);
				frame.Triangle(star[5].x, star[5].y, star[4].x, star[4].y, star[3].x, star[3].y, selected);
				frame.Triangle(star[5].x, star[5].y, star[6].x, star[6].y, star[7].x, star[7].y, selected);
				frame.Triangle(star[7].x, star[7].y, star[1].x, star[1].y, star[3].x, star[3].y, selected);
				frame.Triangle(star[7].x, star[7].y, star[5].x, star[5].y, star[3].x, star[3].y, selected);
			}
			else if (starDrawMode == DrawMode::SECTOR) {
				frame.Triangle(star[7].x, star[7].y, star[0].x, star[0].y, star[1].x, star[1].y, sectorInterpolator);
				frame.Triangle(star[1].x, star[1].y, star[2].x, star[2].y, star[3].x, star[3].y, sectorInterpolator);
				frame.Triangle(star[5].x, star[5].y, star[4].x, star[4].y, star[3].x, star[3].y, sectorInterpolator);
				frame.Triangle(star[5].x, star[5].y, star[6].x, star[6].y, star[7].x, star[7].y, sectorInterpolator);
				frame.Triangle(star[7].x, star[7].y, star[1].x, star[1].y, star[3].x, star[3].y, sectorInterpolator);
				frame.Triangle(star[7].x, star[7].y, star[5].x, star[5].y, star[3].x, star[3].y, sectorInterpolator);
			}
			else if (starDrawMode == DrawMode::RADIAL) {
				frame.Triangle(star[7].x, star[7].y, star[0].x, star[0].y, star[1].x, star[1].y, radialInterpolator);
				frame.Triangle(star[1].x, star[1].y, star[2].x, star[2].y, star[3].x, star[3].y, radialInterpolator);
				frame.Triangle(star[5].x, star[5].y, star[4].x, star[4].y, star[3].x, star[3].y, radialInterpolator);
				frame.Triangle(star[5].x, star[5].y, star[6].x, star[6].y, star[7].x, star[7].y, radialInterpolator);
				frame.Triangle(star[7].x, star[7].y, star[1].x, star[1].y, star[3].x, star[3].y, radialInterpolator);
				frame.Triangle(star[7].x, star[7].y, star[5].x, star[5].y, star[3].x, star[3].y, radialInterpolator);
			}
			else if (starDrawMode == DrawMode::BARYCENTRIC) {
				frame.Triangle(star[7].x, star[7].y, star[0].x, star[0].y, star[1].x, star[1].y, triangleInterpolator);
				frame.Triangle(star[1].x, star[1].y, star[2].x, star[2].y, star[3].x, star[3].y, triangleInterpolator);
				frame.Triangle(star[5].x, star[5].y, star[4].x, star[4].y, star[3].x, star[3].y, triangleInterpolator);
				frame.Triangle(star[5].x, star[5].y, star[6].x, star[6].y, star[7].x, star[7].y, triangleInterpolator);
				frame.Triangle(star[7].x, star[7].y, star[1].x, star[1].y, star[3].x, star[3].y, triangleInterpolator);
				frame.Triangle(star[7].x, star[7].y, star[5].x, star[5].y, star[3].x, star[3].y, triangleInterpolator);
			}
		}
		else {
			float x0 = 0, y0 = 0, x1 = frame.width, y1 = frame.height;
			RadialInterpolator radialInterpolator(x0, y0, x1, y1, { COLOR(0, 255, 0), COLOR(255, 0, 0), COLOR(255, 0, 255) }, global_angle / 5);
			double t = (3 * a) / sqrt(3);
			coordinate triangleA = { C.x, C.y - a };
			coordinate triangleB = { C.x - t / 2, C.y + a / 2 };
			coordinate triangleC = { C.x + t / 2, C.y + a / 2 };
			frame.Triangle(
				triangleA.x + 0.5, triangleA.y + 0.5,
				triangleB.x + 0.5, triangleB.y + 0.5,
				triangleC.x + 0.5, triangleC.y + 0.5,
				radialInterpolator);
		}
		// Рисуем пиксель, на который кликнул пользователь
		if (global_clicked_pixel.X >= 0 && global_clicked_pixel.X < W &&
			global_clicked_pixel.Y >= 0 && global_clicked_pixel.Y < H)
			frame.SetPixel(global_clicked_pixel.X, global_clicked_pixel.Y, { 34, 175, 60 }); // Пиксель зелёного цвета
	}
};

#endif // PAINTER_H