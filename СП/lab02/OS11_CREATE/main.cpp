

#ifdef _WIN64
#pragma comment(lib, "../x64/debug/OS11_HTAPI.lib")
#else
#pragma comment(lib, "../debug/OS11_HTAPI.lib")
#endif
#include "../OS11_HTAPI/pch.h"
#include "../OS11_HTAPI/HT.h"

using namespace std;

wchar_t* getWC(const char* c);

int main(int argc, char* argv[])
{
    if (argc != 6)
    {
        cout << "Usage: OS11_CREATE.exe <capacity> <snapshot_interval_sec> <max_key_length> <max_payload_length> <filename>" << endl;
        return 1;
    }

    ht::HtHandle* ht = nullptr;

    wchar_t* fileName = getWC(argv[5]);
    ht = ht::create(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), fileName);

    if (ht)
    {
        cout << "HT-Storage Created "
            << "filename=";
        wcout << ht->fileName;
        cout << ", snapshotinterval=" << ht->secSnapshotInterval
            << ", capacity=" << ht->capacity
            << ", maxkeylength=" << ht->maxKeyLength
            << ", maxdatalength=" << ht->maxPayloadLength << endl;

        ht::close(ht);
    }
    else
    {
        cout << "-- create: error" << endl;
    }

    delete[] fileName;
}

wchar_t* getWC(const char* c)
{
    wchar_t* wc = new wchar_t[strlen(c) + 1];
    mbstowcs(wc, c, strlen(c) + 1);

    return wc;
}

