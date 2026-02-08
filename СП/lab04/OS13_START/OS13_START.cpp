#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <conio.h> 
#include <string>
#include <cstdlib>

#include "HT.h"

using namespace std;

char* getChar(const char* c)
{
    return _strdup(c);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Usage: OS13_START.exe <filename>" << endl;
        return 1;
    }

    char* fileName = getChar(argv[1]);

    HT::HTHANDLE* ht = nullptr;

    try
    {
        cout << "OS13_START: Attempting to open HT-Storage..." << endl;
        ht = HT::Open(fileName, true);

        if (ht)
        {
            cout << "HT-Storage Start:" << endl;
            cout << "  filename=" << ht->FileName
                << ", snapshotinterval=" << ht->SecSnapshotInterval << endl;
            cout << "  capacity=" << ht->Capacity
                << ", maxkeylength=" << ht->MaxKeyLength
                << ", maxdatalength=" << ht->MaxPayloadLength << endl;

            cout << "--------------------------------------------------------" << endl;
            cout << "Waiting for local key press OR signal from OS13_STOP..." << endl;
            cout << "--------------------------------------------------------" << endl;



            HANDLE hWaitHandles[1] = {
            ht->hStopEvent // Ждем сигнала от OS13_STOP
            };

            DWORD dwResult = WaitForMultipleObjects(
                1,                  // Количество дескрипторов = 1
                hWaitHandles,
                TRUE,               // Ждем только этот объект
                INFINITE
            );

            if (dwResult == WAIT_OBJECT_0) {
                cout << "\nSTOP signal received from OS13_STOP.exe. Stopping..." << endl;
            }
            else {
                cout << "\nWait failed. Error code: " << GetLastError() << ". Stopping..." << endl;
            }

            cout << "\nPerforming synchronous snapshot (Snap)... ";
            if (HT::Snap(ht))
            {
                cout << "SUCCESS." << endl;
            }
            else
            {
                cout << "FAILED: " << HT::GetLastError(ht) << endl;
            }

            if (HT::Close(ht))
            {
                cout << "HT-Storage Closed" << endl;
            }
            else
            {
                cout << "Error closing HT-Storage: " << HT::GetLastError(ht) << endl;
            }
        }
        else
        {
            cout << "Error opening HT-Storage: " << HT::GetLastError(NULL) << endl;
            return 2;
        }

    }
    catch (const std::exception& e)
    {
        cerr << "Critical Error: " << e.what() << endl;
        return 3;
    }
    free(fileName);
    return 0;
}