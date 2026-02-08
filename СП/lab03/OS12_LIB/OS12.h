// OS12.h
#pragma once
#include <objbase.h>

// Макрос для хендла
#define OS12HANDEL void*

namespace OS12
{
    // Инициализация. Создает COM-объект и возвращает хендл (указатель на данные).
    // Если CoCreateInstance(...) не успешен --> бросает исключение (int)HRESULT
    OS12HANDEL Init();

    // Завершение работы. Вызывает Release() и CoUninitialize().
    void Dispose(OS12HANDEL h);

    namespace Adder
    {
        // Методы IAdder
        double Add(OS12HANDEL h, double x, double y); // return x+y
        double Sub(OS12HANDEL h, double x, double y); // return x-y
    }

    namespace Multiplier
    {
        // Методы IMultiplier
        double Mul(OS12HANDEL h, double x, double y); // return x*y
        double Div(OS12HANDEL h, double x, double y); // return x/y
    }
}