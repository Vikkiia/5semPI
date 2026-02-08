
#include "pch.h"
#include "OS12.h"
#include <iostream>
#include "../OS12_COM/Interface.h" 

// 1. Структура для хранения указателей на COM-интерфейсы.
struct OS12_DATA
{
    IUnknown* pUnk = nullptr;
    IAdder* pAdd = nullptr;
    IMultiplier* pMul = nullptr;
    bool comInitialized = false;
};


OS12HANDEL OS12::Init()
{
    OS12_DATA* pData = new OS12_DATA();
    if (!pData)
    {
        throw (int)E_OUTOFMEMORY;
    }

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        if (hr != S_FALSE)
        {
            delete pData;
            throw (int)hr;
        }
    }
    else
    {
        pData->comInitialized = true;
    }

    hr = CoCreateInstance(CLSID_OS12, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pData->pUnk);
    if (FAILED(hr))
    {
        if (pData->comInitialized) CoUninitialize();
        delete pData;
        throw (int)hr;
    }

    hr = pData->pUnk->QueryInterface(IID_IAdder, (void**)&pData->pAdd);
    if (FAILED(hr))
    {
        pData->pUnk->Release();
        if (pData->comInitialized) CoUninitialize();
        delete pData;
        throw (int)hr;
    }

    hr = pData->pUnk->QueryInterface(IID_IMultiplier, (void**)&pData->pMul);
    if (FAILED(hr))
    {
        pData->pAdd->Release();
        if (pData->comInitialized) CoUninitialize();
        delete pData;
        throw (int)hr;
    }


    pData->pUnk->Release();
    pData->pUnk = nullptr;

    return (OS12HANDEL)pData;
}


void OS12::Dispose(OS12HANDEL h)
{
    if (h == nullptr) return;

    OS12_DATA* pData = (OS12_DATA*)h;

    if (pData->pMul) pData->pMul->Release();
    if (pData->pAdd) pData->pAdd->Release();

    bool shouldUninitialize = pData->comInitialized;


    delete pData;

    if (shouldUninitialize)
    {
        CoUninitialize();
    }
}


double OS12::Adder::Add(OS12HANDEL h, double x, double y)
{
    if (h == nullptr) throw (int)E_INVALIDARG;
    OS12_DATA* pData = (OS12_DATA*)h;

    double z = 0.0;
    HRESULT hr = pData->pAdd->Add(x, y, z);

    if (FAILED(hr)) throw (int)hr;
    return z;
}

double OS12::Adder::Sub(OS12HANDEL h, double x, double y)
{
    if (h == nullptr) throw (int)E_INVALIDARG;
    OS12_DATA* pData = (OS12_DATA*)h;

    double z = 0.0;
    HRESULT hr = pData->pAdd->Sub(x, y, z);

    if (FAILED(hr)) throw (int)hr;
    return z;
}


double OS12::Multiplier::Mul(OS12HANDEL h, double x, double y)
{
    if (h == nullptr) throw (int)E_INVALIDARG;
    OS12_DATA* pData = (OS12_DATA*)h;

    double z = 0.0;
    HRESULT hr = pData->pMul->Mul(x, y, z);

    if (FAILED(hr)) throw (int)hr;
    return z;
}

double OS12::Multiplier::Div(OS12HANDEL h, double x, double y)
{
    if (h == nullptr) throw (int)E_INVALIDARG;
    OS12_DATA* pData = (OS12_DATA*)h;

    double z = 0.0;
    HRESULT hr = pData->pMul->Div(x, y, z);

    if (FAILED(hr)) throw (int)hr;
    return z;
}