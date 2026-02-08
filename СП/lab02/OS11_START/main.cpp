// Файл реализации OS11_START.cpp

#ifdef _WIN64
#pragma comment(lib, "../x64/debug/OS11_HTAPI.lib")
#else
#pragma comment(lib, "../debug/OS11_HTAPI.lib")
#endif

#include <conio.h>
#include "../OS11_HTAPI/pch.h"
#include "../OS11_HTAPI/HT.h"

using namespace std;

wchar_t* getWC(const char* c);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Usage: OS11_START.exe <filename>" << endl;
        return 1;
    }

    ht::HtHandle* ht = nullptr;

    wchar_t* fileName = getWC(argv[1]);
    ht = ht::open(fileName);

    if (ht)
    {

        cout << "HT-Storage Start "
            << "filename=";
        wcout << ht->fileName;
        cout << ", snapshotinterval=" << ht->secSnapshotInterval
            << ", capacity=" << ht->capacity
            << ", maxkeylength=" << ht->maxKeyLength
            << ", maxdatalength=" << ht->maxPayloadLength << endl;

        while (!kbhit())
            SleepEx(0, TRUE);

        cout << "\nSnapshot..." << endl;
        if (!ht::snap(ht))
        {
            cout << "-- snap: error" << endl;
        }

        ht::close(ht);
        cout << "HT-Storage Closed" << endl;
    }
    else
    {
        cout << "-- open: error" << endl;
    }
    delete[] fileName;
}

wchar_t* getWC(const char* c)
{
    wchar_t* wc = new wchar_t[strlen(c) + 1];
    mbstowcs(wc, c, strlen(c) + 1);

    return wc;
}