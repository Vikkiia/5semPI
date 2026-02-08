// Файл: OS13_0402.cpp

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <Windows.h> 

#include "HT.h" 

using namespace std;

string intToString(int number);
string getTimeStamp();

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Usage: OS13_0402.exe <filename>" << endl;
        return 1;
    }

    HT::HTHANDLE* ht = nullptr;

    const char* fileName = argv[1];

    cout << getTimeStamp() << " [INFO] Attempting to open HT-Storage (via OS13_HTCOM): " << fileName << endl;

    ht = HT::Open(fileName, false);

    if (!ht)
    {
        cout << getTimeStamp() << " [ERROR] Failed to open HT-Storage. Check if OS13_START is running." << endl;
        return 1;
    }

    cout << getTimeStamp() << " [SUCCESS] Connected to HT-Storage. Starting insertion loop." << endl;

    int payloadValue = 0;

    srand(static_cast<unsigned int>(time(NULL)));

    while (true)
    {
        try
        {
            int numberKey = rand() % 50;
            string keyStr = intToString(numberKey);


            HT::Element* element = new HT::Element(
                keyStr.c_str(),
                keyStr.length() + 1,
                &payloadValue,
                sizeof(int)
            );

            if (HT::Insert(ht, element))
            {
                cout << getTimeStamp() << " [INSERT] Key: " << keyStr << ", Status: SUCCESS" << endl;
            }
            else
            {
                cout << getTimeStamp() << " [WARNING] Key: " << keyStr << ", Status: ERROR (" << HT::GetLastError(ht) << ")" << endl;
            }

            delete element;

            Sleep(1000);
        }
        catch (...)
        {
            cout << getTimeStamp() << " [FATAL] An unknown exception occurred in the loop." << endl;
            break;
        }
    }

    if (ht) {
        HT::Close(ht);
    }
    return 0;
}

string intToString(int number)
{
    stringstream convert;
    convert << number; 
    return convert.str();
}

string getTimeStamp()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeinfo);
    return string(buffer);
}