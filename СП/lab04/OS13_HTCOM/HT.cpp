#include "pch.h"
#include "framework.h"
#include "HT.h"
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdint>
#include <vector>

using namespace std;

//  WaitableTimer 
#define SECOND (10000000LL) 

namespace HT {

    
    BOOL Snap(const HTHANDLE* h);

    
    static void setError(HTHANDLE* ht, const char* msg);

  
    VOID CALLBACK SnapTimerCallback(LPVOID prm, DWORD, DWORD)
    {
        HTHANDLE* ht = (HTHANDLE*)prm;
        if (Snap(ht))
            // cout << "-- [HT::SnapTimerCallback] Snapshot success" << endl;
            ; 
    }


    static const int HDR_MAGIC_OFF = 0;//поле расположено в самом начале данных
    static const int HDR_VERSION_OFF = 8;//смещение для версии структуры
    static const int HDR_CAP_OFF = 12;
    static const int HDR_SNAPINT_OFF = 16;//смещение для интервала снимков
    static const int HDR_MAXKEY_OFF = 20;
    static const int HDR_MAXPAY_OFF = 24;
    static const int HDR_LASTSNAP_OFF = 28;
    static const int HEADER_SIZE = 36;

    enum BucketState : uint8_t { B_EMPTY = 0, B_OCCUPIED = 1, B_DELETED = 2 };

  

    static void setError(HTHANDLE* ht, const char* msg) {
        strncpy_s(ht->LastErrorMessage, msg, _TRUNCATE);
    }

    static uint64_t fnv1a(const uint8_t* data, size_t len) {
        const uint64_t FNV_OFFSET = 1469598103934665603ull;
        const uint64_t FNV_PRIME = 1099511628211ull;
        uint64_t h = FNV_OFFSET;
        for (size_t i = 0; i < len; ++i) { h ^= data[i]; h *= FNV_PRIME; }//операция XOR между текущим хешем h и текущим байтом данных
        return h;
    }

    static inline int bucketSize(const HTHANDLE* ht) {
        return 1 + 4 + 4 + ht->MaxKeyLength + ht->MaxPayloadLength;
    }
    static inline uint64_t fileSize(const HTHANDLE* ht) {
        return uint64_t(HEADER_SIZE) + uint64_t(ht->Capacity) * uint64_t(bucketSize(ht));
    }

    static inline uint8_t* basePtr(const HTHANDLE* ht) {
        return reinterpret_cast<uint8_t*>(ht->Addr);//адрес данных хэш-таблицы
    }
    static inline uint8_t* headerPtr(const HTHANDLE* ht) {
        return basePtr(ht);
    }
    static inline uint8_t* bucketPtr(const HTHANDLE* ht, int idx) {
        return basePtr(ht) + HEADER_SIZE + size_t(idx) * size_t(bucketSize(ht));
    }

    
    static inline BucketState b_state(uint8_t* b) { return (BucketState)b[0]; }
    static inline void b_set_state(uint8_t* b, BucketState s) { b[0] = (uint8_t)s; }
    static inline int b_keylen(uint8_t* b) { int v; memcpy(&v, b + 1, 4); return v; }
    static inline void b_set_keylen(uint8_t* b, int v) { memcpy(b + 1, &v, 4); }
    static inline int b_paylen(uint8_t* b) { int v; memcpy(&v, b + 5, 4); return v; }
    static inline void b_set_paylen(uint8_t* b, int v) { memcpy(b + 5, &v, 4); }
    static inline uint8_t* b_key_area(const HTHANDLE* ht, uint8_t* b) { return b + 9; }
    static inline uint8_t* b_pay_area(const HTHANDLE* ht, uint8_t* b) { return b + 9 + ht->MaxKeyLength; }

    
    static int probe_find(const HTHANDLE* ht, const void* key, int keylen, int& foundIndex) {
       
        const uint8_t* k = reinterpret_cast<const uint8_t*>(key);
        const int cap = ht->Capacity;
        uint64_t h = fnv1a(k, size_t(keylen));
        int firstDeleted = -1;
        foundIndex = -1;

        for (int i = 0; i < cap; ++i) {
            int idx = int((h + (uint64_t)i) % (uint64_t)cap);
            uint8_t* b = bucketPtr(ht, idx);
            BucketState st = b_state(b);

            if (st == B_EMPTY) {
                return (firstDeleted != -1) ? firstDeleted : idx;
            }
            if (st == B_DELETED) {
                if (firstDeleted == -1) firstDeleted = idx;
                continue;
            }
            // OCCUPIED
            int klen = b_keylen(b);
            if (klen == keylen) {
                if (memcmp(b_key_area(ht, b), k, keylen) == 0) {
                    foundIndex = idx;
                    return idx;
                }
            }
        }
        return (firstDeleted != -1) ? firstDeleted : -1;
    }


   

    HTHANDLE::HTHANDLE() {
        Capacity = 0;
        SecSnapshotInterval = 0;
        MaxKeyLength = 0;
        MaxPayloadLength = 0;
        File = NULL;
        FileMapping = NULL;
        Addr = NULL;
        LastErrorMessage[0] = '\0';
        lastsnaptime = 0;
        FileName[0] = '\0';


        snapshotTimer = NULL;
        mutex = NULL;
    }

    HTHANDLE::HTHANDLE(int capacity, int interval, int keylen, int payloadlen, const char filename[512]) {
        Capacity = capacity;
        SecSnapshotInterval = interval;
        MaxKeyLength = keylen;
        MaxPayloadLength = payloadlen;
        File = NULL;
        FileMapping = NULL;
        Addr = NULL;
        LastErrorMessage[0] = '\0';
        lastsnaptime = 0;
        strcpy_s(FileName, filename);

        snapshotTimer = NULL;
        mutex = NULL;
    }

    Element::Element() {

        key = nullptr;

        keylength = 0;

        payload = nullptr;

        payloadlength = 0;

    }



    Element::Element(const void* k, int kl) {

        keylength = kl;

        payload = nullptr;

        payloadlength = 0;

        key = malloc(kl);

        memcpy((void*)key, k, kl);

    }



    Element::Element(const void* k, int kl, const void* p, int pl) {

        keylength = kl;

        payloadlength = pl;

        key = malloc(kl);

        payload = malloc(pl);

        memcpy((void*)key, k, kl);

        memcpy((void*)payload, p, pl);

    }



    Element::Element(Element* oldelement, const void* newpayload, int newpayloadlength) {

        keylength = oldelement->keylength;

        key = malloc(keylength);

        memcpy((void*)key, oldelement->key, keylength);

        payloadlength = newpayloadlength;

        payload = malloc(payloadlength);

        memcpy((void*)payload, newpayload, payloadlength);

    }



    static void init_header(HTHANDLE* ht) {

        uint8_t* h = headerPtr(ht);

        const char magic[8] = { 'H','T','S','N','A','P','1','0' };

        memcpy(h + HDR_MAGIC_OFF, magic, 8);

        uint32_t ver = 1;

        memcpy(h + HDR_VERSION_OFF, &ver, 4);

        memcpy(h + HDR_CAP_OFF, &ht->Capacity, 4);

        memcpy(h + HDR_SNAPINT_OFF, &ht->SecSnapshotInterval, 4);

        memcpy(h + HDR_MAXKEY_OFF, &ht->MaxKeyLength, 4);

        memcpy(h + HDR_MAXPAY_OFF, &ht->MaxPayloadLength, 4);

        int64_t ts = (int64_t)time(NULL);

        memcpy(h + HDR_LASTSNAP_OFF, &ts, 8);

        ht->lastsnaptime = (time_t)ts;

    }



    static bool read_header(HTHANDLE* ht) {

        uint8_t* h = headerPtr(ht);

        char magic[8]; memcpy(magic, h + HDR_MAGIC_OFF, 8);

        const char expected[8] = { 'H','T','S','N','A','P','1','0' };

        if (memcmp(magic, expected, 8) != 0) { setError(ht, "Open: bad magic"); return false; }



        uint32_t ver = 0; memcpy(&ver, h + HDR_VERSION_OFF, 4);

        if (ver != 1) { setError(ht, "Open: bad version"); return false; }



        memcpy(&ht->Capacity, h + HDR_CAP_OFF, 4);

        memcpy(&ht->SecSnapshotInterval, h + HDR_SNAPINT_OFF, 4);

        memcpy(&ht->MaxKeyLength, h + HDR_MAXKEY_OFF, 4);

        memcpy(&ht->MaxPayloadLength, h + HDR_MAXPAY_OFF, 4);

        int64_t ts = 0; memcpy(&ts, h + HDR_LASTSNAP_OFF, 8);

        ht->lastsnaptime = (time_t)ts;



        return true;

    }



    static void init_buckets(HTHANDLE* ht) {

        const int cap = ht->Capacity;

        const int bsz = bucketSize(ht);

        uint8_t* p = basePtr(ht) + HEADER_SIZE;

        for (int i = 0; i < cap; ++i) {

            p[0] = (uint8_t)B_EMPTY;

            memset(p + 1, 0, size_t(bsz - 1));

            p += bsz;

        }

    }



    HTHANDLE* Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]) {
        HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);


        ht->File = CreateFileA(FileName, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_WRITE_THROUGH, NULL);
        if (ht->File == INVALID_HANDLE_VALUE) { setError(ht, "Create: CreateFile failed"); delete ht; return nullptr; }


        LARGE_INTEGER sz; sz.QuadPart = fileSize(ht);
        LARGE_INTEGER newpos;
        if (!SetFilePointerEx(ht->File, sz, &newpos, FILE_BEGIN) || !SetEndOfFile(ht->File)) {
            setError(ht, "Create: SetEndOfFile failed");
            CloseHandle(ht->File); delete ht; return nullptr;
        }


        ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, 0, 0, NULL);
        if (!ht->FileMapping) { setError(ht, "Create: CreateFileMapping failed"); CloseHandle(ht->File); delete ht; return nullptr; }
        ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (!ht->Addr) { setError(ht, "Create: MapViewOfFile failed"); CloseHandle(ht->FileMapping); CloseHandle(ht->File); delete ht; return nullptr; }

        init_header(ht);
        init_buckets(ht);

        UnmapViewOfFile(ht->Addr);
        CloseHandle(ht->FileMapping);
        CloseHandle(ht->File);
        ht->Addr = NULL;
        ht->FileMapping = NULL;
        ht->File = NULL;

        return ht;
    }



    HTHANDLE* Open(const char FileName[512], bool isStartMode) {
        HTHANDLE* ht = new HTHANDLE();
        strcpy_s(ht->FileName, FileName);

        const char* const STATIC_NAME_MAPPING = "Global\\OS13_StorageFileMapping";
        const char* const STATIC_NAME_MUTEX = "Global\\OS13_StorageMutex";
        const char* const STATIC_NAME_EVENT = "Global\\OS13_StopEvent";

        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = FALSE;

        if (isStartMode) {
            // --- РЕЖИМ START (OS13_START) ---

            ht->File = CreateFileA(FileName, GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
            if (ht->File == INVALID_HANDLE_VALUE) { setError(ht, "Open [Start]: CreateFile failed"); delete ht; return nullptr; }

            
            ht->FileMapping = CreateFileMappingA(
                ht->File,
                &sa, 
                PAGE_READWRITE,
                0,
                0,
                STATIC_NAME_MAPPING
            );
            if (!ht->FileMapping) { setError(ht, "Open [Start]: CreateFileMapping failed"); CloseHandle(ht->File); delete ht; return nullptr; }

            
            ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            if (!ht->Addr) { setError(ht, "Open [Start]: MapViewOfFile failed"); CloseHandle(ht->FileMapping); CloseHandle(ht->File); delete ht; return nullptr; }

            
            if (!read_header(ht)) {
                UnmapViewOfFile(ht->Addr); CloseHandle(ht->FileMapping); CloseHandle(ht->File); delete ht; return nullptr;
            }

            
            ht->mutex = CreateMutexA(&sa, FALSE, STATIC_NAME_MUTEX);
            if (ht->mutex == NULL) { setError(ht, "Open [Start]: CreateMutex failed"); Close(ht); return nullptr; }

            
            ht->hStopEvent = CreateEventA(&sa, TRUE, FALSE, STATIC_NAME_EVENT);
            if (ht->hStopEvent == NULL) { setError(ht, "Open [Start]: CreateEvent failed"); Close(ht); return nullptr; }

            
            ht->snapshotTimer = CreateWaitableTimer(0, FALSE, 0);
            if (ht->snapshotTimer == NULL) { setError(ht, "Open [Start]: CreateWaitableTimer failed"); Close(ht); return nullptr; }

            LARGE_INTEGER Li{};
            Li.QuadPart = -(SECOND * ht->SecSnapshotInterval);

            if (!SetWaitableTimer(ht->snapshotTimer, &Li, ht->SecSnapshotInterval * 1000, (PTIMERAPCROUTINE)SnapTimerCallback, ht, FALSE))
            {
                setError(ht, "Open [Start]: SetWaitableTimer failed"); Close(ht); return nullptr;
            }

            return ht;

        }
        else {
            // CLIENT

            
            ht->FileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, STATIC_NAME_MAPPING);
            if (!ht->FileMapping) { setError(ht, "Open [Client]: OpenFileMapping failed (Storage not started?)"); delete ht; return nullptr; }

            
            ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            if (!ht->Addr) { setError(ht, "Open [Client]: MapViewOfFile failed"); CloseHandle(ht->FileMapping); delete ht; return nullptr; }

            
            if (!read_header(ht)) {
                UnmapViewOfFile(ht->Addr); CloseHandle(ht->FileMapping); delete ht; return nullptr;
            }

            
            ht->mutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, STATIC_NAME_MUTEX);
            if (ht->mutex == NULL) { setError(ht, "Open [Client]: OpenMutex failed"); Close(ht); return nullptr; }

          
            ht->hStopEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, STATIC_NAME_EVENT);
            if (ht->hStopEvent == NULL) { setError(ht, "Open [Client]: OpenEvent failed"); Close(ht); return nullptr; }

            
            return ht;
        }
    }

    BOOL Snap(const HTHANDLE* h) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);

        // Ждем освобождения мьютекса
        DWORD waitResult = WaitForSingleObject(ht->mutex, INFINITE);
        if (waitResult != WAIT_OBJECT_0) {
            
            setError(ht, "Snap: Failed to acquire mutex");
            return FALSE;
        }

        BOOL result = TRUE;
        if (!FlushViewOfFile(ht->Addr, 0)) { setError(ht, "Snap: FlushViewOfFile failed"); result = FALSE; }
        if (ht->File && ht->File != INVALID_HANDLE_VALUE && !FlushFileBuffers(ht->File)) {
            setError(ht, "Snap: FlushFileBuffers failed");
            result = FALSE;
        }
        int64_t ts = (int64_t)time(NULL);
        memcpy(headerPtr(ht) + HDR_LASTSNAP_OFF, &ts, 8);
        ht->lastsnaptime = (time_t)ts;

        // Освобождаем мьютекс
        if (!ReleaseMutex(ht->mutex)) {
        }

        return result;
    }

    BOOL Close(const HTHANDLE* h) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!ht) return FALSE;

        
        if (ht->snapshotTimer && ht->snapshotTimer != INVALID_HANDLE_VALUE) {
            CancelWaitableTimer(ht->snapshotTimer);
            CloseHandle(ht->snapshotTimer);
        }

      
        Snap(ht);

        if (ht->File == NULL && ht->hStopEvent && ht->hStopEvent != INVALID_HANDLE_VALUE) {
            SetEvent(ht->hStopEvent);
        }
        
        if (ht->Addr) UnmapViewOfFile(ht->Addr);
        if (ht->FileMapping) CloseHandle(ht->FileMapping);
        // Закрываем ht->File только если он открыт (только для START-сервера)
        if (ht->File && ht->File != INVALID_HANDLE_VALUE) CloseHandle(ht->File);

        if (ht->mutex && ht->mutex != INVALID_HANDLE_VALUE) CloseHandle(ht->mutex);
        if (ht->hStopEvent && ht->hStopEvent != INVALID_HANDLE_VALUE) CloseHandle(ht->hStopEvent);

        delete ht;
        return TRUE;
    }

    
    BOOL Insert(const HTHANDLE* h, const Element* e) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!e || !e->key || e->keylength <= 0) { setError(ht, "Insert: bad key"); return FALSE; }
        if (!e->payload || e->payloadlength < 0) { setError(ht, "Insert: bad payload"); return FALSE; }
        if (e->keylength > ht->MaxKeyLength) { setError(ht, "Insert: key too long"); return FALSE; }
        if (e->payloadlength > ht->MaxPayloadLength) { setError(ht, "Insert: payload too long"); return FALSE; }

        // Ждем мьютекса
        DWORD waitResult = WaitForSingleObject(ht->mutex, INFINITE);
        if (waitResult != WAIT_OBJECT_0) { setError(ht, "Insert: Failed to acquire mutex"); return FALSE; }

        int found = -1;
        int idx = probe_find(ht, e->key, e->keylength, found);
        if (idx < 0) { setError(ht, "Insert: no slot"); ReleaseMutex(ht->mutex); return FALSE; }
        if (found >= 0) { setError(ht, "Insert: duplicate key"); ReleaseMutex(ht->mutex); return FALSE; }

        uint8_t* b = bucketPtr(ht, idx);
        b_set_state(b, B_OCCUPIED);
        b_set_keylen(b, e->keylength);
        b_set_paylen(b, e->payloadlength);
        memcpy(b_key_area(ht, b), e->key, size_t(e->keylength));
        if (e->payloadlength > 0)
            memcpy(b_pay_area(ht, b), e->payload, size_t(e->payloadlength));

       
        ReleaseMutex(ht->mutex);
        return TRUE;
    }

    
    Element* Get(const HTHANDLE* h, const Element* e) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!e || !e->key || e->keylength <= 0) { setError(ht, "Get: bad key"); return nullptr; }
        if (e->keylength > ht->MaxKeyLength) { setError(ht, "Get: key too long"); return nullptr; }

        // Ждем мьютекса
        DWORD waitResult = WaitForSingleObject(ht->mutex, INFINITE);
        if (waitResult != WAIT_OBJECT_0) { setError(ht, "Get: Failed to acquire mutex"); return nullptr; }

        int found = -1;
        (void)probe_find(ht, e->key, e->keylength, found);

        Element* out = nullptr;
        if (found >= 0)
        {
            uint8_t* b = bucketPtr(ht, found);
            int paylen = b_paylen(b);
            uint8_t* pay = b_pay_area(ht, b);

            out = new Element(e->key, e->keylength);
            if (paylen > 0) {
                out->payload = malloc(size_t(paylen));
                memcpy((void*)out->payload, pay, size_t(paylen));
                out->payloadlength = paylen;
            }
            else {
                out->payload = nullptr;
                out->payloadlength = 0;
            }
        }
        else {
            setError(ht, "Get: key not found");
        }

      
        ReleaseMutex(ht->mutex);
        return out;
    }

    
    BOOL Update(const HTHANDLE* h, const Element* olde, const void* newpayload, int newpayloadlength) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!olde || !olde->key || olde->keylength <= 0) { setError(ht, "Update: bad key"); return FALSE; }
        if (newpayloadlength < 0) { setError(ht, "Update: bad payload length"); return FALSE; }
        if (olde->keylength > ht->MaxKeyLength) { setError(ht, "Update: key too long"); return FALSE; }
        if (newpayloadlength > ht->MaxPayloadLength) { setError(ht, "Update: payload too long"); return FALSE; }

        // Ждем мьютекса
        DWORD waitResult = WaitForSingleObject(ht->mutex, INFINITE);
        if (waitResult != WAIT_OBJECT_0) { setError(ht, "Update: Failed to acquire mutex"); return FALSE; }

        int found = -1;
        (void)probe_find(ht, olde->key, olde->keylength, found);
        if (found < 0) { setError(ht, "Update: key not found"); ReleaseMutex(ht->mutex); return FALSE; }

        uint8_t* b = bucketPtr(ht, found);
        b_set_paylen(b, newpayloadlength);
        if (newpayloadlength > 0)
            memcpy(b_pay_area(ht, b), newpayload, size_t(newpayloadlength));
        else
            memset(b_pay_area(ht, b), 0, size_t(ht->MaxPayloadLength));

       
        ReleaseMutex(ht->mutex);
        return TRUE;
    }

   
    BOOL Delete(const HTHANDLE* h, const Element* e) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!e || !e->key || e->keylength <= 0) { setError(ht, "Delete: bad key"); return FALSE; }
        if (e->keylength > ht->MaxKeyLength) { setError(ht, "Delete: key too long"); return FALSE; }

        // Ждем мьютекса
        DWORD waitResult = WaitForSingleObject(ht->mutex, INFINITE);
        if (waitResult != WAIT_OBJECT_0) { setError(ht, "Delete: Failed to acquire mutex"); return FALSE; }

        int found = -1;
        (void)probe_find(ht, e->key, e->keylength, found);
        if (found < 0) { setError(ht, "Delete: key not found"); ReleaseMutex(ht->mutex); return FALSE; }

        uint8_t* b = bucketPtr(ht, found);
        b_set_state(b, B_DELETED);
        b_set_keylen(b, 0);
        b_set_paylen(b, 0);
        memset(b_key_area(ht, b), 0, size_t(ht->MaxKeyLength + ht->MaxPayloadLength));

        
        ReleaseMutex(ht->mutex);
        return TRUE;
    }
    static string intToString_internal(int number) {
        char buffer[16]; // Достаточно для 64-битного int
        snprintf(buffer, sizeof(buffer), "%d", number);
        return buffer;
    }



    
    char* GetLastError(HTHANDLE* ht) {
        return ht->LastErrorMessage;
    }

    
    void print(const Element* element) { /* ... */ }

} // namespace HT