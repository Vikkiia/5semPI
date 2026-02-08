#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>

#ifdef _WIN64
#pragma comment(lib, "../x64/debug/OS11_HTAPI.lib") 
#else
#pragma comment(lib, "../debug/OS11_HTAPI.lib") 
#endif

#include "../OS11_HTAPI/pch.h"
#include "../OS11_HTAPI/HT.h"

using namespace std;

string intToString(int number);
string getTimeStamp(); 

int main(int argc, char* argv[])
{
    

    ht::HtHandle* ht = ht::open(L"storage.ht", true);

    if (!ht)
    {
        cout << getTimeStamp() << " [ERROR] Failed to open HT-Storage. Check if OS11_START is running." << endl;
        return 1;
    }

    cout << getTimeStamp() << " [SUCCESS] Connected to HT-Storage. Starting insertion loop." << endl;
    
    int payloadValue = 0; 
    
    srand(time(NULL));

    while (true) 
    {
        try
        {
            int numberKey = rand() % 50;
            string keyStr = intToString(numberKey);
            
            ht::Element* element = ht::createInsertElement(
                keyStr.c_str(), keyStr.length() + 1, 
                &payloadValue, sizeof(int)
            );

            if (ht::insert(ht, element))
            {
                cout << getTimeStamp() << " [INSERT] Key: " << keyStr << ", Status: SUCCESS" << endl;
            }
            else
            {
                cout << getTimeStamp() << " [WARNING] Key: " << keyStr << ", Status: ERROR (" << ht::getLastError(ht) << ")" << endl;
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

    ht::close(ht);
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
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeinfo);
    return string(buffer);
}