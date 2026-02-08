#include "pch.h"
#include <Windows.h>
#include "Interface.h"
#include "Registry.h"


// Глобальный счетчик ссылок на модуль (сколько объектов сейчас живет)
long g_lObjs = 0;
// Глобальный счетчик блокировок сервера
long g_lLocks = 0;


class COS12 : public IAdder, public IMultiplier
{
protected:
    long m_lRef;

public:
    COS12() : m_lRef(1) { InterlockedIncrement(&g_lObjs); }
    ~COS12() { InterlockedDecrement(&g_lObjs); }

    // IUnknown
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppv)
    {
        if (riid == IID_IUnknown)
            *ppv = static_cast<IAdder*>(this);
        else if (riid == IID_IAdder)
            *ppv = static_cast<IAdder*>(this);
        else if (riid == IID_IMultiplier)
            *ppv = static_cast<IMultiplier*>(this);
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

    // IAdder
    HRESULT __stdcall Add(const double x, const double y, double& z)
    {
        z = x + y;
        return S_OK;
    }
    HRESULT __stdcall Sub(const double x, const double y, double& z)
    {
        z = x - y;
        return S_OK;
    }

    // IMultiplier
    HRESULT __stdcall Mul(const double x, const double y, double& z)
    {
        z = x * y;
        return S_OK;
    }
    HRESULT __stdcall Div(const double x, const double y, double& z)
    {
        if (y == 0) return E_INVALIDARG; 
        z = x / y;
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
    {
        if (pUnknownOuter != NULL) return CLASS_E_NOAGGREGATION;
        COS12* p = new COS12();
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
    if (rclsid != CLSID_OS12) return CLASS_E_CLASSNOTAVAILABLE;

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

    if (dwResult == 0)
    {
        return E_FAIL;
    }

    return RegisterServer(szModule,
        CLSID_OS12,
        L"OS12.MathComponent",
        L"OS12.Math",
        L"OS12.Math.1");
}

extern "C" HRESULT __stdcall DllUnregisterServer()
{
    return UnregisterServer(CLSID_OS12, L"OS12.Math", L"OS12.Math.1");
}