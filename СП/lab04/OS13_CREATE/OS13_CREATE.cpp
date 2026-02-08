#include <iostream>
#include <Windows.h>
#include "HT.h" 

using namespace std;


// OS13_CREATE.exe <FileName> <Capacity> <SnapshotInterval> <MaxKeyLength> <MaxPayloadLength>

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cout << "Usage: OS13_CREATE.exe <FileName> <Capacity> <SnapshotInterval> <MaxKeyLength> <MaxPayloadLength>" << endl;
        return 1;
    }

    const char* fileName = argv[1];
    int capacity = atoi(argv[2]);
    int snapInterval = atoi(argv[3]);
    int maxKey = atoi(argv[4]);
    int maxPay = atoi(argv[5]);

    HT::HTHANDLE* ht = NULL;

    try {
        cout << "OS13_CREATE: Attempting to create HT-Storage..." << endl;

        ht = HT::Create(capacity, snapInterval, maxKey, maxPay, fileName);

        if (ht) {
            cout << "HT-Storage Created successfully:" << endl;
            cout << "  filename=" << ht->FileName
                << ", snapshotinterval=" << ht->SecSnapshotInterval << endl;
            cout << "  capacity=" << ht->Capacity
                << ", maxkeylength=" << ht->MaxKeyLength
                << ", maxdatalength=" << ht->MaxPayloadLength << endl;

           
            if (HT::Close(ht)) {
                cout << "HT-Storage closed." << endl;
            }
            else {
                cout << "Warning: Failed to close HT-Storage." << endl;
            }

        }
        else {
            cout << "Error creating HT-Storage: " << HT::GetLastError(NULL) << endl;
            return 2;
        }

    }
    catch (int e) {
        cerr << "Critical Error (HRESULT): " << hex << e << dec << endl;
        return 3;
    }

    return 0;
}