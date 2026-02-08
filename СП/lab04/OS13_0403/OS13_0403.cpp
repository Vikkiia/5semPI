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

int main(int argc, char* argv[]) {
	
	srand(time(NULL));

	if (argc != 2) {
		cout << "Usage: OS13_0403.exe <filename>" << endl;
		return 1;
	}


	char* fileName = argv[1];
	HT::HTHANDLE* ht = nullptr;

	try {
		cout << "OS13_0403: Attempting to open HT-Storage in Client Mode..." << endl;

		
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

		
			HT::Element* element = new HT::Element((void*)c_key, key_len);

			cout << "Attempting to DELETE key: " << numberKey << " (Length: " << key_len << ")... ";

			
			if (HT::Delete(ht, element)) {
				cout << "SUCCESS." << endl;
			}
			else {
				
				cout << "FAILED. Reason: " << HT::GetLastError(ht) << endl;
			}

			
			delete element;
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