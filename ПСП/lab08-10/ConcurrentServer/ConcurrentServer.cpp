#define _CRT_SECURE_NO_WARNINGS

#include "Global.h"
#include "AcceptServer.h"
#include "DispathServer.h"
#include "GarbageCleaner.h"
#include "ConsolePipe.h"
#include "ResponseServer.h"
#include "tchar.h"


int _tmain(int argc, _TCHAR* argv[]) {
    setlocale(LC_ALL, "Russian");

    try {

        if (argc > 1) {
            int tmp = atoi(argv[1]);
            if (tmp >= 0 && tmp <= 65535) {
                port = atoi(argv[1]);
                cout << "TCP port specified: " << port << endl;
            }
            else {
                cout << "Invalid TCP port" << endl;
            }
        }
        else {
            cout << "Using default TCP port: " << port << endl;
        }

        if (argc > 2) {
            int tmp = atoi(argv[2]);
            if (tmp >= 0 && tmp <= 65535) {
                uport = atoi(argv[2]);
                cout << "UDP port specified: " << uport << endl;
            }
            else {
                cout << "Invalid UDP port" << endl;
            }
        }
        else {
            cout << "Using default UDP port: " << uport << endl;
        }

        if (argc > 3) {
            dllname = argv[3];
        }

        if (argc > 4) {
            npname = argv[4];
            cout << "Named pipe specified: " << npname << endl;
        }
        else cout << "Using default named pipe: " << npname << endl;

        if (argc > 5) {
            ucall = argv[5];
            cout << "Callsign specified: " << ucall << endl;
        }
        cout << "Using default callsign: " << ucall << endl;

        srand((unsigned)time(NULL));

        volatile TalkersCommand  cmd = START;

        InitializeCriticalSection(&scListContact);
        InitializeCriticalSection(&scListLoadedLibraries);

        // Загружаем начальную библиотеку
        if (!LoadLibraryDLL(dllname)) {
            cout << "Warning: Failed to load initial DLL: " << dllname << endl;
        }
        
        // Сохраняем для совместимости со старым кодом
        if (!LoadedLibraries.empty()) {
            st1 = LoadedLibraries.begin()->hModule;
            sss = LoadedLibraries.begin()->sssFunc;
        }

        hAcceptServer = CreateThread(NULL, NULL, AcceptServer, (LPVOID)&cmd, NULL, NULL);
        HANDLE hDispathServer = CreateThread(NULL, NULL, DispathServer, (LPVOID)&cmd, NULL, NULL);

        HANDLE hConsolePipe = CreateThread(NULL, NULL, ConsolePipe, (LPVOID)&cmd, NULL, NULL);
        HANDLE hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner, (LPVOID)&cmd, NULL, NULL);

        HANDLE hResponseServer = CreateThread(NULL, NULL, ResponseServer, (LPVOID)&cmd, NULL, NULL);

        SetThreadPriority(hAcceptServer, THREAD_PRIORITY_HIGHEST);
        SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_BELOW_NORMAL);
        SetThreadPriority(hConsolePipe, THREAD_PRIORITY_NORMAL);

        SetThreadPriority(hResponseServer, THREAD_PRIORITY_ABOVE_NORMAL);
        SetThreadPriority(hDispathServer, THREAD_PRIORITY_NORMAL);

        WaitForSingleObject(hAcceptServer, INFINITE);
        WaitForSingleObject(hDispathServer, INFINITE);
        WaitForSingleObject(hConsolePipe, INFINITE);
        WaitForSingleObject(hGarbageCleaner, INFINITE);
        WaitForSingleObject(hResponseServer, INFINITE);

        CloseHandle(hAcceptServer);
        CloseHandle(hDispathServer);
        CloseHandle(hGarbageCleaner);
        CloseHandle(hConsolePipe);
        CloseHandle(hResponseServer);

        DeleteCriticalSection(&scListContact);
        
        // Выгружаем все загруженные библиотеки
        EnterCriticalSection(&scListLoadedLibraries);
        for (ListLoadedLibraries::iterator it = LoadedLibraries.begin(); it != LoadedLibraries.end(); ++it) {
            FreeLibrary(it->hModule);
            cout << "Library unloaded on exit: " << it->libName << endl;
        }
        LoadedLibraries.clear();
        LeaveCriticalSection(&scListLoadedLibraries);
        DeleteCriticalSection(&scListLoadedLibraries);
    }
    catch (...) {
        cout << "error" << endl;
    }

    system("pause");
    return 0;
}
