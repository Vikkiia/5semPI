#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
double cosv1v2(CMatrix& one, CMatrix& two);
class CPyramid
{
private:
	CMatrix Vertices; // Координаты вершин
	void GetRect(CMatrix& Vert, CRectD& RectView);

public:
	CPyramid();
	void Draw(CDC& dc, CMatrix& P, CRect& RW);
	void Draw1(CDC& dc, CMatrix& P, CRect& RW);
	void ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF color, CMatrix& Light);

};

void Normalize(CMatrix& V)
{
	double len = sqrt(V(0) * V(0) + V(1) * V(1) + V(2) * V(2));
	if (len > 1e-9)
	{
		V(0) /= len;
		V(1) /= len;
		V(2) /= len;
	}
}



CPyramid::CPyramid()
{
	Vertices.RedimMatrix(4, 6);	// ABC    — нижнее основание
								// A'B'C' — верхнее основание
	/*       A                   B                    C                   A'                  B'                  C'      */
	Vertices(0, 0) = 0;	Vertices(0, 1) = 0;  Vertices(0, 2) = 6; Vertices(0, 3) = 0; Vertices(0, 4) = 0; Vertices(0, 5) = 3;
	Vertices(1, 0) = 6; Vertices(1, 1) = 0;  Vertices(1, 2) = 0; Vertices(1, 3) = 3; Vertices(1, 4) = 0; Vertices(1, 5) = 0;
	Vertices(2, 0) = 0; Vertices(2, 1) = 0;  Vertices(2, 2) = 0; Vertices(2, 3) = 6; Vertices(2, 4) = 6; Vertices(2, 5) = 6;
	Vertices(3, 0) = 1; Vertices(3, 1) = 1;  Vertices(3, 2) = 1; Vertices(3, 3) = 1; Vertices(3, 4) = 1; Vertices(3, 5) = 1;
}

void CPyramid::GetRect(CMatrix& Vert, CRectD& RectView) //коорд прям-ка, охват проекцию пирамиды на XY ВСК
{
	CMatrix V = Vert.GetRow(0);               // x - координаты
	double xMin = V.MinElement();
	double xMax = V.MaxElement();
	V = Vert.GetRow(1);                       // y - координаты
	double yMin = V.MinElement();
	double yMax = V.MaxElement();
	RectView.SetRectD(xMin, yMax, xMax, yMin);
}

void CPyramid::Draw(CDC& dc, CMatrix& PView, CRect& RW)
// Рисует пирамиду БЕЗ удаления невидимых ребер
// Самостоятельный пересчет координат из мировых в оконные (MM_TEXT)
// dc - ссылка на класс CDC MFC
// P - координаты точки наблюдения в мировой сферической системе
// координат
// (r,fi(град.), q(град.))
// RW - область в окне для отображения	
{
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // Матрица (4x4)
	CMatrix ViewVert = MV * Vertices; // Координаты вершин пирамиды в видовой СК
	CRectD RectView; // проекция - охватывающий прямоугольник
	GetRect(ViewVert, RectView); // Получаем охватывающий прямоугольник по к-там вершин
	CMatrix MW = SpaceToWindow(RectView, RW); // Матрица (3x3) для пересчета координат из мировых в оконные

	CPoint MasVert[6]; // Масив оконных координат вершин,
	CMatrix V(3);
	V(2) = 1;
	// Цикл по количеству вершин – вычисляем оконные коодинаты
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i); // x в видовой
		V(1) = ViewVert(1, i); // y в видовой
		V = MW * V;            // Оконные координаты точки
		MasVert[i].x = (int)V(0); // оконная к-ты х
		MasVert[i].y = (int)V(1); // оконная к-та у
	}

	// Рисуем
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	dc.MoveTo(MasVert[3]);
	// Ребра НИЖНЕЙ грани
	for (int i = 0; i < 3; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	dc.LineTo(MasVert[0]);
	dc.MoveTo(MasVert[5]);
	// Ребра ВЕРХНЕЙ грани
	for (int i = 3; i < 6; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	// Ребра БОКОВЫХ граней
	for (int i = 0; i < 3; i++)
	{
		dc.MoveTo(MasVert[i]);
		dc.LineTo(MasVert[i + 3]);
	}
	dc.SelectObject(pOldPen);
}
void CPyramid::Draw1(CDC& dc, CMatrix& PView, CRect& RW)
// Рисует пирамиду С УДАЛЕНИЕМ невидимых ребер
{
	CMatrix ViewCart = SphereToCart(PView); // Декартовы координаты точки наблюдения
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // Матрица (4x4) пересчета точки из мировой системы координат в видовую
	CMatrix ViewVert = MV * Vertices; // Координаты вершин пирамиды в видовой СК
	CRectD RectView;
	GetRect(ViewVert, RectView); // Получаем охватывающий прямоугольник
	CMatrix MW = SpaceToWindow(RectView, RW); // Матрица (3x3) для пересчета

	CPoint MasVert[6]; // Масив оконных координат вершин,
	CMatrix V(3);
	V(2) = 1;
	// Цикл по количеству вершин – вычисляем оконные коодинаты
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
		V = MW * V;            // Оконные координаты точки
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// Рисуем
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	CBrush Brush(RGB(255, 0, 0));
	CBrush* pOldBrush = dc.SelectObject(&Brush);
	CMatrix R1(3), R2(3), VN(3);
	double sm;
	for (int i = 0; i < 3; i++)
	{
		CMatrix VE = Vertices.GetCol(i + 3, 0, 2); // Вершина E
		int k;
		if (i == 2)
			k = 0;
		else
			k = i + 1;
		R1 = Vertices.GetCol(i, 0, 2); // Текущая точка основания
		R2 = Vertices.GetCol(k, 0, 2); // Следующая точка основания
		CMatrix V1 = R2 - R1;          // Вектор – ребро в основании
		CMatrix V2 = VE - R1;          // Вектор – ребро к вершине
		VN = VectorMult(V2, V1);       // Вектор ВНЕШНЕЙ нормали
		sm = ScalarMult(VN, ViewCart); // Скалярное произведение 

		if (sm >= 0) // Грань видима – рисуем боковую грань
		{
			dc.MoveTo(MasVert[i]);
			dc.LineTo(MasVert[k]);
			dc.LineTo(MasVert[k + 3]);
			dc.LineTo(MasVert[i + 3]);
			dc.LineTo(MasVert[i]);
		}
	}

	VN = VectorMult(R1, R2);
	sm = ScalarMult(VN, ViewCart);
	if (sm >= 0) // Основание
	{
		dc.Polygon(MasVert, 3);
	}
	else
	{
		CBrush* topBrush = new CBrush(RGB(0, 0, 255));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert + 3, 3);	// верхнее основание
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}


//--------------------------------------------------------ColorDraw----------------------------------------------------------------------------------------------------------

void CPyramid::ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF color, CMatrix& Light)
{
    // Извлекаем компоненты цвета RGB
    double Red = (double)GetRValue(color);
    double Green = (double)GetGValue(color);
    double Blue = (double)GetBValue(color);

    // Коэффициенты освещения
    const double ambient = 0.18;         // фоновая составляющая
    const double diffuse_coef = 0.82;    // коэффициент диффузного освещения
    const double EPS = 1e-7;             // эпсилон для сравнения с нулём

    // Преобразуем сферические координаты камеры и света в декартовы
    CMatrix ViewCart = SphereToCart(PView);
    CMatrix LightCart = SphereToCart(Light);

    // Построение матрицы видовой системы координат
    CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));

    // Преобразуем вершины в видовую СК
    CMatrix ViewVert = MV * Vertices;

    // Получаем габаритный прямоугольник в видовой СК
    CRectD RectView;
    GetRect(ViewVert, RectView);

    // Матрица перехода из видовой СК в экранную (окно)
    CMatrix MW = SpaceToWindow(RectView, RW);

    // Преобразуем вершины в экранные координаты
    CPoint MasVert[6];
    CMatrix V(3);
    V(2) = 1; // однородная координата
    for (int i = 0; i < 6; ++i)
    {
        V(0) = ViewVert(0, i);
        V(1) = ViewVert(1, i);
        V = MW * V;
        MasVert[i].x = (int)round(V(0));
        MasVert[i].y = (int)round(V(1));
    }

    // Структура для хранения информации о грани
    struct Face { std::vector<int> idx; double depth; double intensity; bool visible; };
    std::vector<Face> faces;

    // Безопасная нормализация вектора
    auto safeNormalize = [&](CMatrix& M) -> bool {
        double len = ModVec(M);
        if (len < 1e-9) return false;
        M(0) /= len; M(1) /= len; M(2) /= len;
        return true;
        };

    // Переворот нормали, если она "смотрит внутрь"
    auto flipIfNeeded = [&](CMatrix& N, const CMatrix& pointToCamera) {
        double v = cosv1v2(N, (CMatrix)pointToCamera);
        if (v < 0) {
            N(0) = -N(0); N(1) = -N(1); N(2) = -N(2);
            v = -v;
        }
        return v;
        };

    // ---------- БОКОВЫЕ ГРАНИ (квадраты) ----------
    for (int i = 0; i < 3; ++i)
    {
        int k = (i == 2) ? 0 : i + 1;
        Face f;
        f.idx = { i, k, k + 3, i + 3 };

        // точки в мировой СК
        CMatrix R1 = Vertices.GetCol(i, 0, 2);
        CMatrix R2 = Vertices.GetCol(k, 0, 2);
        CMatrix VE = Vertices.GetCol(i + 3, 0, 2);

        CMatrix V1 = R2 - R1;
        CMatrix V2 = VE - R1;
        CMatrix N = VectorMult(V2, V1); // нормаль 
        if (!safeNormalize(N)) {
            // вырожденная грань — пропускаем
            f.visible = false;
            f.intensity = ambient;
            f.depth = 0.0;
            faces.push_back(f);
            continue;
        }

        CMatrix viewVec = ViewCart - R1;
        if (!safeNormalize(viewVec)) {
            f.visible = false;
            f.intensity = ambient;
            f.depth = 0.0;
            faces.push_back(f);
            continue;
        }

        double visCos = flipIfNeeded(N, viewVec); // при необходимости переворачивает N
        f.visible = (visCos > -1e-4); // стабильная проверка видимости

        CMatrix lightVec = LightCart - R1;
        if (!safeNormalize(lightVec)) {
            // если свет в той же точке — оставим ambient
            f.intensity = ambient;
        } else {
            double Lcos = cosv1v2(N, lightVec);
            if (Lcos < 0.0) Lcos = 0.0;
            double intensity = ambient + diffuse_coef * Lcos;
            if (intensity > 1.0) intensity = 1.0;
            f.intensity = intensity;
        }

        double zsum = 0.0;
        for (int id : f.idx) zsum += ViewVert(2, id);
        f.depth = zsum / (double)f.idx.size();

        faces.push_back(f);
    }

    // ---------- НИЖНЕЕ ОСНОВАНИЕ (0,1,2) ----------
    {
        Face f;
        f.idx = { 0,1,2 };
        CMatrix R1 = Vertices.GetCol(0, 0, 2);
        CMatrix R2 = Vertices.GetCol(1, 0, 2);
        CMatrix R3 = Vertices.GetCol(2, 0, 2);
        CMatrix N = VectorMult(R2 - R1, R3 - R1); // нормаль основания
        if (!safeNormalize(N)) {
            f.visible = false; f.intensity = ambient; f.depth = ViewVert(2,0); faces.push_back(f);
        } else {
            CMatrix viewVec = ViewCart - R1;
            if (!safeNormalize(viewVec)) { f.visible = false; f.intensity = ambient; f.depth = ViewVert(2,0); faces.push_back(f); }
            else {
                double visCos = flipIfNeeded(N, viewVec);
                f.visible = (visCos > -1e-4);
                CMatrix lightVec = LightCart - R1;
                if (!safeNormalize(lightVec)) f.intensity = ambient;
                else {
                    double Lcos = cosv1v2(N, lightVec);
                    if (Lcos < 0.0) Lcos = 0.0;
                    double intensity = ambient + diffuse_coef * Lcos;
                    if (intensity > 1.0) intensity = 1.0;
                    f.intensity = intensity;
                }
                f.depth = (ViewVert(2,0) + ViewVert(2,1) + ViewVert(2,2)) / 3.0;
                faces.push_back(f);
            }
        }
    }

    // ---------- ВЕРХНЕЕ ОСНОВАНИЕ (3,4,5) ----------
    {
        Face f;
        f.idx = { 3,4,5 };
        CMatrix R1 = Vertices.GetCol(3, 0, 2);
        CMatrix R2 = Vertices.GetCol(4, 0, 2);
        CMatrix R3 = Vertices.GetCol(5, 0, 2);
        CMatrix N = VectorMult(R2 - R1, R3 - R1); // нормаль верхнего основания
        if (!safeNormalize(N)) {
            f.visible = false; f.intensity = ambient; f.depth = ViewVert(2,3); faces.push_back(f);
        } else {
            CMatrix viewVec = ViewCart - R1;
            if (!safeNormalize(viewVec)) { f.visible = false; f.intensity = ambient; f.depth = ViewVert(2,3); faces.push_back(f); }
            else {
                double visCos = flipIfNeeded(N, viewVec);
                f.visible = (visCos > -1e-4);

                CMatrix lightVec = LightCart - R1;
                if (!safeNormalize(lightVec)) f.intensity = ambient;
                else {
                    double Lcos = cosv1v2(N, lightVec);
                    if (Lcos < 0.0) Lcos = 0.0;
                    double intensity = ambient + diffuse_coef * Lcos;
                    if (intensity > 1.0) intensity = 1.0;
                    f.intensity = intensity;
                }
                f.depth = (ViewVert(2,3) + ViewVert(2,4) + ViewVert(2,5)) / 3.0;
                faces.push_back(f);
            }
        }
    }

    // Сортировка и рисование (дальние — первыми)
    std::sort(faces.begin(), faces.end(), [](const Face& a, const Face& b) {
        return a.depth < b.depth;
    });

    for (const Face& f : faces)
    {
        if (!f.visible) continue;

        std::vector<CPoint> poly;
        for (int id : f.idx) poly.push_back(MasVert[id]);

        int rcol = (int)round(Red * f.intensity);
        int gcol = (int)round(Green * f.intensity);
        int bcol = (int)round(Blue * f.intensity);
        rcol = max(0, min(255, rcol));
        gcol = max(0, min(255, gcol));
        bcol = max(0, min(255, bcol));

        CBrush brush(RGB(rcol, gcol, bcol));
        CPen pen(PS_SOLID, 1, RGB(max(0, (int)(rcol * 0.6)), max(0, (int)(gcol * 0.6)), max(0, (int)(bcol * 0.6))));
        CBrush* pOldBrush = dc.SelectObject(&brush);
        CPen* pOldPen = dc.SelectObject(&pen);

        dc.Polygon(poly.data(), (int)poly.size());

        dc.SelectObject(pOldPen);
        dc.SelectObject(pOldBrush);
    }
}

double cosv1v2(CMatrix& V1, CMatrix& V2)
{
	double modV1 = ModVec(V1);
	double modV2 = ModVec(V2);

	if (modV1 < 1e-7 || modV2 < 1e-7)
		return 0.0; // граница почти вырожденная — считаем неосвещённой

	double cos_f = ScalarMult(V1, V2) / (modV1 * modV2);
	return cos_f;
}

