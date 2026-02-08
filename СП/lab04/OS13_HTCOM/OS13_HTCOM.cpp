#include "pch.h"
#include <Windows.h>
#include <string>
#include <vector>
#include "Interface.h"
#include "Registry.h"
#include "HT.h"          


long g_lObjs = 0;
long g_lLocks = 0;

std::string WStringToString(const WCHAR* wstr) {
    if (!wstr) return "";
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    
    //int WideCharToMultiByte(
    //    UINT CodePage,              // Кодовая страница
    //    DWORD dwFlags,              // Флаги преобразования
    //    LPCWSTR lpWideCharStr,      // Указатель на исходную строку (широкие символы)
    //    int cchWideChar,            // Количество символов в исходной строке
    //    LPSTR lpMultiByteStr,       // Указатель на целевую строку (многобайтовую)
    //    int cbMultiByte,            // Размер целевого буфера (в байтах)
    //    LPCSTR lpDefaultChar,       // Символ по умолчанию для необрабатываемых символов
    //    LPBOOL lpUsedDefaultChar    // Выводит информацию о том, использовался ли символ по умолчанию
    //);


    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, &strTo[0], size_needed, NULL, NULL);
    if (!strTo.empty() && strTo.back() == '\0') strTo.pop_back();
    return strTo;
}

// (для возврата строк)
std::wstring StringToWString(const char* str) {
    if (!str) return L"";
    int size_needed = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, str, -1, &wstrTo[0], size_needed);
    return wstrTo;
}


class COS13 : public IHT
{
protected:
    long m_lRef;

public:
    COS13() : m_lRef(1) { InterlockedIncrement(&g_lObjs); }
    ~COS13() { InterlockedDecrement(&g_lObjs); }

    // IUnknown 
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppv)
    {
        if (riid == IID_IUnknown || riid == IID_IHT)
            *ppv = static_cast<IHT*>(this);
        else
        {
            *ppv = NULL;
            return E_NOINTERFACE;
        }
        ((IUnknown*)*ppv)->AddRef();
        return S_OK;
    }

    ULONG __stdcall AddRef()
    {
        return InterlockedIncrement(&m_lRef);
    }

    ULONG __stdcall Release()
    {
        ULONG res = InterlockedDecrement(&m_lRef);
        if (res == 0) delete this;
        return res;
    }



    HRESULT __stdcall Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const WCHAR* FileName, unsigned __int64& hHandle)
    {
        std::string sFile = WStringToString(FileName);
        HT::HTHANDLE* ht = HT::Create(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, sFile.c_str());

        if (ht == nullptr) return E_FAIL;

        hHandle = reinterpret_cast<unsigned __int64>(ht);
        return S_OK;
    }

    HRESULT __stdcall Open(const WCHAR* FileName, bool isStartMode, unsigned __int64& hHandle)
    {
        std::string sFile = WStringToString(FileName);
        HT::HTHANDLE* ht = HT::Open(sFile.c_str(), isStartMode);

        if (ht == nullptr) return E_FAIL;

        hHandle = reinterpret_cast<unsigned __int64>(ht);
        return S_OK;
    }

    HRESULT __stdcall Snap(unsigned __int64 hHandle)
    {
        HT::HTHANDLE* ht = reinterpret_cast<HT::HTHANDLE*>(hHandle);//хранит указатель на объект
        if (!ht) return E_INVALIDARG;

        return HT::Snap(ht) ? S_OK : E_FAIL;
    }

    HRESULT __stdcall Close(unsigned __int64 hHandle)
    {
        HT::HTHANDLE* ht = reinterpret_cast<HT::HTHANDLE*>(hHandle);
        if (!ht) return E_INVALIDARG;

        return HT::Close(ht) ? S_OK : E_FAIL;
    }

    HRESULT __stdcall Insert(unsigned __int64 hHandle, const WCHAR* key, int keyLen, const WCHAR* payload, int payloadLen)
    {
        HT::HTHANDLE* ht = reinterpret_cast<HT::HTHANDLE*>(hHandle);
        if (!ht) return E_INVALIDARG;

        std::string sKey = WStringToString(key);
        std::string sPayload = WStringToString(payload);

        HT::Element el(sKey.c_str(), (int)sKey.length() + 1, sPayload.c_str(), (int)sPayload.length() + 1);

        BOOL res = HT::Insert(ht, &el);

      
        free((void*)el.key);
        free((void*)el.payload);

        return res ? S_OK : E_FAIL;
    }

    HRESULT __stdcall Delete(unsigned __int64 hHandle, const WCHAR* key, int keyLen)
    {
        HT::HTHANDLE* ht = reinterpret_cast<HT::HTHANDLE*>(hHandle);
        if (!ht) return E_INVALIDARG;

        std::string sKey = WStringToString(key);
        HT::Element el(sKey.c_str(), (int)sKey.length() + 1);

        BOOL res = HT::Delete(ht, &el);
        free((void*)el.key);

        return res ? S_OK : E_FAIL;
    }

    HRESULT __stdcall Get(unsigned __int64 hHandle, const WCHAR* key, int keyLen, WCHAR** resultPayload, int& resultPayloadLen)
    {
        HT::HTHANDLE* ht = reinterpret_cast<HT::HTHANDLE*>(hHandle);
        if (!ht) return E_INVALIDARG;

        std::string sKey = WStringToString(key);
        HT::Element elKey(sKey.c_str(), (int)sKey.length() + 1);

        HT::Element* found = HT::Get(ht, &elKey);
        free((void*)elKey.key); 

        if (found)
        {
            // Конвертируем payload обратно в WCHAR для клиента
            std::wstring wRes = StringToWString((const char*)found->payload);

           
            size_t sizeBytes = (wRes.length() + 1) * sizeof(WCHAR);
            *resultPayload = (WCHAR*)CoTaskMemAlloc(sizeBytes);
            if (*resultPayload)
            {
                memcpy(*resultPayload, wRes.c_str(), sizeBytes);
                resultPayloadLen = (int)wRes.length();
            }

           
            free((void*)found->key);
            free((void*)found->payload);
            delete found;
            return S_OK;
        }
        else
        {
            *resultPayload = NULL;
            return E_FAIL; // Не найдено
        }
    }

    HRESULT __stdcall Update(unsigned __int64 hHandle, const WCHAR* key, int keyLen, const WCHAR* newPayload, int newPayloadLen)
    {
        HT::HTHANDLE* ht = reinterpret_cast<HT::HTHANDLE*>(hHandle);
        if (!ht) return E_INVALIDARG;

        std::string sKey = WStringToString(key);
        std::string sPay = WStringToString(newPayload);
        HT::Element elKey(sKey.c_str(), (int)sKey.length() + 1);

        BOOL res = HT::Update(ht, &elKey, sPay.c_str(), (int)sPay.length() + 1);

        free((void*)elKey.key);
        return res ? S_OK : E_FAIL;
    }

    HRESULT __stdcall GetLastErrorMessage(unsigned __int64 hHandle, WCHAR** errorMsg)
    {
        HT::HTHANDLE* ht = reinterpret_cast<HT::HTHANDLE*>(hHandle);
        if (!ht) return E_INVALIDARG;

        char* err = HT::GetLastError(ht);
        std::wstring wErr = StringToWString(err);

        size_t sizeBytes = (wErr.length() + 1) * sizeof(WCHAR);
        *errorMsg = (WCHAR*)CoTaskMemAlloc(sizeBytes);
        if (*errorMsg)
        {
            memcpy(*errorMsg, wErr.c_str(), sizeBytes);
        }
        return S_OK;
    }
};


class CFactory : public IClassFactory
{
protected:
    long m_lRef;
public:
    CFactory() : m_lRef(1) {}
    ~CFactory() {}
      
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppv)
    {
        if (riid == IID_IUnknown || riid == IID_IClassFactory)
        {
            *ppv = static_cast<IClassFactory*>(this);
            ((IUnknown*)*ppv)->AddRef();
            return S_OK;
        }
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    ULONG __stdcall AddRef() { return InterlockedIncrement(&m_lRef); }
    ULONG __stdcall Release()
    {
        ULONG res = InterlockedDecrement(&m_lRef);
        if (res == 0) delete this;
        return res;
    }

    HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void** ppv)
    {//один объект может быть включен в другой
        if (pUnknownOuter != NULL) return CLASS_E_NOAGGREGATION;

        
        COS13* p = new COS13();
        if (p == NULL) return E_OUTOFMEMORY;

        HRESULT hr = p->QueryInterface(riid, ppv);
        p->Release();
        return hr;
    }

    HRESULT __stdcall LockServer(BOOL bLock)
    {
        if (bLock) InterlockedIncrement(&g_lLocks);
        else InterlockedDecrement(&g_lLocks);
        return S_OK;
    }
};



extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    if (rclsid != CLSID_OS13) return CLASS_E_CLASSNOTAVAILABLE;

    CFactory* pFactory = new CFactory();
    if (pFactory == NULL) return E_OUTOFMEMORY;

    HRESULT hr = pFactory->QueryInterface(riid, ppv);
    pFactory->Release();
    return hr;
}

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
    if (g_lObjs == 0 && g_lLocks == 0) return S_OK;
    else return S_FALSE;
}

extern "C" HRESULT __stdcall DllRegisterServer()
{
    WCHAR szModule[512];
    HMODULE hModule = NULL;

    if (!GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)DllRegisterServer,
        &hModule))
    {
        return E_FAIL;
    }

    DWORD dwResult = GetModuleFileNameW(hModule, szModule, sizeof(szModule) / sizeof(WCHAR));
    if (dwResult == 0) return E_FAIL;

    // Регистрируем OS13
    return RegisterServer(szModule,
        CLSID_OS13,
        L"OS13.HTComponent",
        L"OS13.HT",
        L"OS13.HT.1");
}

extern "C" HRESULT __stdcall DllUnregisterServer()
{
    return UnregisterServer(CLSID_OS13, L"OS13.HT", L"OS13.HT.1");
}