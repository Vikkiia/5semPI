#pragma once
#include <Windows.h>
#include <objbase.h> 

// Генерация новых GUID для OS13
// {77A8215F-23A4-45F2-8716-116503923231}
static const CLSID CLSID_OS13 =
{ 0x77a8215f, 0x23a4, 0x45f2, { 0x87, 0x16, 0x11, 0x65, 0x03, 0x92, 0x32, 0x31 } };

// {E86F7652-3209-4D30-81A4-850117075253}
static const IID IID_IHT =
{ 0xe86f7652, 0x3209, 0x4d30, { 0x81, 0xa4, 0x85, 0x01, 0x17, 0x07, 0x52, 0x53 } };

// Основной интерфейс для работы с HT
interface IHT : IUnknown
{
    // Аналог HT::Create
    virtual HRESULT __stdcall Create(
        int Capacity,
        int SecSnapshotInterval,
        int MaxKeyLength,
        int MaxPayloadLength,
        const WCHAR * FileName,      // Используем WCHAR для COM
        unsigned __int64& hHandle   // Возвращаем хендл (адрес HTHANDLE)
    ) = 0;

    // Аналог HT::Open
    virtual HRESULT __stdcall Open(
        const WCHAR* FileName,
        bool isStartMode,
        unsigned __int64& hHandle
    ) = 0;

    // Аналог HT::Snap
    virtual HRESULT __stdcall Snap(unsigned __int64 hHandle) = 0;

    // Аналог HT::Close
    virtual HRESULT __stdcall Close(unsigned __int64 hHandle) = 0;

    // Аналог HT::Insert
    virtual HRESULT __stdcall Insert(
        unsigned __int64 hHandle,
        const WCHAR* key,           // Ключ строкой WCHAR
        int keyLen,
        const WCHAR* payload,       // Payload тоже WCHAR для простоты примера
        int payloadLen
    ) = 0;

    // Аналог HT::Delete
    virtual HRESULT __stdcall Delete(
        unsigned __int64 hHandle,
        const WCHAR* key,
        int keyLen
    ) = 0;

    // Аналог HT::Get
    virtual HRESULT __stdcall Get(
        unsigned __int64 hHandle,
        const WCHAR* key,
        int keyLen,
        WCHAR** resultPayload,     // Возврат через указатель на строку
        int& resultPayloadLen
    ) = 0;

    // Аналог HT::Update
    virtual HRESULT __stdcall Update(
        unsigned __int64 hHandle,
        const WCHAR* key,
        int keyLen,
        const WCHAR* newPayload,
        int newPayloadLen
    ) = 0;

    // Аналог HT::GetLastError
    virtual HRESULT __stdcall GetLastErrorMessage(
        unsigned __int64 hHandle,
        WCHAR** errorMsg
    ) = 0;
};