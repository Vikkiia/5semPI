
#include <iostream>
#include <iomanip>
#include <Windows.h> // Для обработки HRESULT

#pragma comment(lib, "OS12_LIB.lib")
#include "OS12.h"

void PrintHResultError(HRESULT hr)
{
    std::wcout << L"COM ERROR: 0x" << std::hex << hr;

    LPWSTR errorText = NULL;
    if (FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&errorText,
        0,
        NULL) != 0)
    {
        std::wcout << L" (" << errorText << L")";
        LocalFree(errorText);
    }
    std::wcout << std::endl;
}

int main()
{
    std::cout << std::fixed << std::setprecision(6);

    OS12HANDEL h1 = nullptr;
    OS12HANDEL h2 = nullptr;

    try
    {
        std::cout << "Initializing h1..." << std::endl;
        h1 = OS12::Init();
        std::cout << "Initializing h2..." << std::endl;
        h2 = OS12::Init();

        std::cout << "OS12::Adder::Add(h1, 2, 3) = " << OS12::Adder::Add(h1, 2, 3) << "\n";
        std::cout << "OS12::Adder::Add(h2, 2, 3) = " << OS12::Adder::Add(h2, 2, 3) << "\n";

        std::cout << "OS12::Adder::Sub(h1, 2, 3) = " << OS12::Adder::Sub(h1, 2, 3) << "\n";
        std::cout << "OS12::Adder::Sub(h2, 2, 3) = " << OS12::Adder::Sub(h2, 2, 3) << "\n";

        std::cout << "OS12::Multiplier::Mul(h1, 2, 3) = " << OS12::Multiplier::Mul(h1, 2, 3) << "\n";
        std::cout << "OS12::Multiplier::Mul(h2, 2, 3) = " << OS12::Multiplier::Mul(h2, 2, 3) << "\n";

        std::cout << "OS12::Multiplier::Div(h1, 2, 3) = " << OS12::Multiplier::Div(h1, 2, 3) << "\n";
        std::cout << "OS12::Multiplier::Div(h2, 2, 3) = " << OS12::Multiplier::Div(h2, 2, 3) << "\n";

    }
    catch (int hr)
    {
        std::cerr << "\n--- FATAL EXCEPTION ---" << std::endl;
        PrintHResultError((HRESULT)hr);

        if ((HRESULT)hr == E_INVALIDARG)
        {
            std::cerr << "Error successfully caught: Division by zero or invalid argument." << std::endl;
        }
    }
    catch (...)
    {
        std::cerr << "\n--- UNKNOWN EXCEPTION ---" << std::endl;
    }

    if (h1)
    {
        OS12::Dispose(h1);
        h1 = nullptr; 
    }
    if (h2)
    {
        OS12::Dispose(h2);
        h2 = nullptr; 
    }


    std::cout << "\nProgram finished." << std::endl;

    return 0;
}