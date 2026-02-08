#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#include <cstdlib> 
#include <ctime>   
#include "HT.h" 

using namespace std;



string intToString(int number) {
    stringstream convert;
    convert << number;
    return convert.str();
}

int charToInt(const char* str) {
    if (!str || *str == '\0') return 0;
    return atoi(str);
}

string incrementPayload(const char* str) {
    int oldNumberPayload = charToInt(str);
    int newNumberPayload = oldNumberPayload + 1;
    string newPayload = intToString(newNumberPayload);
    return newPayload;
}



int main(int argc, char* argv[]) {
    
    srand(time(NULL));

    if (argc != 2) {
        cout << "Usage: OS13_0404.exe <filename>" << endl;
        return 1;
    }

    char* fileName = argv[1];
    HT::HTHANDLE* ht = nullptr;

    try {
        cout << "OS13_0404: Attempting to open HT-Storage in Client Mode..." << endl;

        
        ht = HT::Open(fileName, false);

        if (!ht) {
            cout << "Error opening HT-Storage: " << HT::GetLastError(NULL) << endl;
            return 2;
        }

        cout << "-- Open: SUCCESS. Connected to: " << ht->FileName << endl;

       
        while (true) {

           
            int numberKey = rand() % 50;
            string key_str = intToString(numberKey);

            const char* c_key = key_str.c_str();
            int key_len = (int)key_str.length() + 1;

            
            HT::Element* searchElement = new HT::Element((void*)c_key, key_len);

            
            DWORD waitResult = WaitForSingleObject(ht->mutex, INFINITE);
            if (waitResult != WAIT_OBJECT_0) {
                cout << "CRITICAL ERROR: Failed to acquire mutex." << endl;
                delete searchElement;
                continue;
            }

        
            HT::Element* foundElement = HT::Get(ht, searchElement);

            if (foundElement)
            {
                
                string newPayload_str = incrementPayload((const char*)foundElement->payload);

                
                if (HT::Update(ht, foundElement, newPayload_str.c_str(), (int)newPayload_str.length() + 1))
                    cout << key_str << ": GET/UPDATE success. New value: " << newPayload_str << endl;
                else
                    cout << key_str << ": UPDATE failed. Reason: " << HT::GetLastError(ht) << endl;
            }
            else {
                cout << key_str << ": GET failed. Reason: " << HT::GetLastError(ht) << endl;
            }

            
            if (foundElement) delete foundElement;

           
            ReleaseMutex(ht->mutex);

            delete searchElement;
            Sleep(500);
        }
    }
    catch (const exception& e) {
        cerr << "Critical Error: " << e.what() << endl;
        if (ht) HT::Close(ht);
        return 3;
    }

    if (ht) HT::Close(ht);
    return 0;
}