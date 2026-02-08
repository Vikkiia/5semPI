//Global
#pragma once
#include "locale.h"
#include <iostream>
#include "Winsock2.h"
#pragma comment(lib,"WS2_32.lib")
#include "Windows.h"
#include <string>
#include <list>
#include <time.h>
#include "Error.h"

using std::string;
using std::list;

int port = 2000; //используемый порт (значение по умолчанию 2000)
int uport = 2000; //udp-порт, используемый ResponseServer
const char* dllname = "Service_Library"; //название загружаемой библиотеки 
const char* npname = "cpipe"; //имя именованного канала, используемое ConsolePipe
const char* ucall = "HELLO";				//позывной сервера, используемый ResponseServer
//int squirt = AS_SQUIRT; //текущее максимальное количество клиентов одновременно
HANDLE(*sss)(char*, LPVOID); //дескриптор, используемый для импорта функции из dll
HANDLE hAcceptServer; //дескриптор AcceptServer
HMODULE st1; //дескриптор dll (старая переменная, оставлена для совместимости)

// Структура для хранения загруженной DLL библиотеки
struct LoadedLibrary {
	char libName[256];                    // имя библиотеки
	HMODULE hModule;                      // дескриптор библиотеки
	HANDLE(*sssFunc)(char*, LPVOID);      // указатель на функцию SSS из библиотеки
};

typedef list<LoadedLibrary> ListLoadedLibraries;
ListLoadedLibraries LoadedLibraries;      // список загруженных библиотек
CRITICAL_SECTION scListLoadedLibraries;   // критическая секция для синхронизации доступа к списку библиотек

// Функции для работы с библиотеками
bool LoadLibraryDLL(const char* libName);
bool UnloadLibraryDLL(const char* libName);
HANDLE FindServiceInLibraries(char* serviceName, LPVOID prm);

HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL); //дескриптор события


int AS_SQUIRT = 10; //максимальное количество клиентов одновременно define
enum TalkersCommand { START, STOP, EXIT, STATISTICS, WAIT, SHUTDOWN, GETCOMMAND }; //допустимые команды RConsole

//статистика подключений
volatile LONG Accept = 0;  //количество подключений
volatile LONG Fail = 0;    //неудачные подключения
volatile LONG Finished = 0;//завершенные удачно
volatile LONG Timeout = 0; //завершенные по таймауту
volatile LONG ClientServiceNumber = 0;    //подключены в текущий момент


#pragma region  ListContact

CRITICAL_SECTION scListContact; //критическая секция

struct Contact         // элемент списка подключений       
{
	//состояние сервера подключения
	enum TE {
		EMPTY,              //пустой элемент списка подключений 
		ACCEPT,             //подключен, но не обслуживается
		CONTACT             //передан обслуживающему серверу  
	}    type;

	// состояние обслуживающего сервера
	enum ST {
		WORK,               //идет обмен данными с клиентом
		ABORT,              //обслуживающий сервер завершился ненормально 
		TIMEOUT,            //обслуживающий сервер завершился по таймеру 
		FINISH              //обслуживающий сервер завершился  нормально 
	}      sthread;

	// для хранения параметров соединения
	SOCKET      s;         //сокет для обмена данными с клиентом
	SOCKADDR_IN prms;      //параметры  сокета 
	int         lprms;     //длина prms 


	HANDLE      hthread;   //дескриптор потока  EchoServer
	HANDLE      htimer;    //дескриптор ожидающего таймера, позволяющего ограничить время работы обслуживающего процесса

	bool        TimerOff;  //метка срабатывания таймера
	bool        CloseConn;  //метка завершения обслуживания

	//диагностирующего сообщения и символических имен обрабатывающих потоков.
	char msg[50];           //сообщение 
	char srvname[15];       //имя обслуживающего потока 

	HANDLE hAcceptServer;// Handle обслуживающего потока

	Contact(TE t = EMPTY, const char* namesrv = "") //конструктор 
	{
		memset(&prms, 0, sizeof(SOCKADDR_IN));
		lprms = sizeof(SOCKADDR_IN); //размер сокета
		type = t;
		strcpy_s(srvname, namesrv);
		msg[0] = 0;
		CloseConn = false;
		TimerOff = false;
	};

	void SetST(ST sth, const char* m = "") {
		sthread = sth;
		strcpy_s(msg, m);
	}
};
typedef list<Contact> ListContact;
ListContact Contacts; // список подключений  

#pragma endregion

//асинхронная функция при срабатывании таймера
void CALLBACK ASWTimer(LPVOID Lprm, DWORD, DWORD) {

	char obuf[50] = "Close: timeout;";
	Contact* client = (Contact*)Lprm; //преобразуем переданный параметр

	EnterCriticalSection(&scListContact); 
	client->TimerOff = true; 	//ставим метку срабатывания таймера
	client->sthread = Contact::TIMEOUT;
	LeaveCriticalSection(&scListContact); 	

	if ((send(client->s, obuf, sizeof(obuf) + 1, NULL)) == SOCKET_ERROR) throw  SetErrorMsgText("Send:", WSAGetLastError());

	SYSTEMTIME stt;
	GetLocalTime(&stt);
	printf("%d.%d.%d %d:%02d Timeout ", stt.wDay, stt.wMonth, stt.wYear, stt.wHour, stt.wMinute);
	cout << client->srvname << ";" << endl;
}


void CALLBACK Test(LPVOID Lprm, DWORD, DWORD) {

	char obuf[50] = "Close: Service unavailable;";
	Contact* client = (Contact*)Lprm; 

	EnterCriticalSection(&scListContact); 
	client->TimerOff = true; 	//ставим метку срабатывания таймера
	client->sthread = Contact::FINISH;
	LeaveCriticalSection(&scListContact); 

	if ((send(client->s, obuf, sizeof(obuf) + 1, NULL)) == SOCKET_ERROR) throw  SetErrorMsgText("Send:", WSAGetLastError());

	SYSTEMTIME stt;
	GetLocalTime(&stt);
	
	printf("%d.%d.%d %d:%02d Service unavailable ", stt.wDay, stt.wMonth, stt.wYear, stt.wHour, stt.wMinute);
	cout << client->srvname << ";" << endl;
}

// Асинхронная функция запуска обслуживающего потока
void CALLBACK ASStartMessage(DWORD Lprm) {
	Contact* client = (Contact*)Lprm;
	/*EnterCriticalSection(&scListContact);*/
	// Ставим метку срабатывания таймера
	char* sn = client->srvname;
	// Покидаем критическую секцию
	//LeaveCriticalSection(&scListContact);
	// Структура времени
	SYSTEMTIME stt;

	GetLocalTime(&stt);

	printf("%d.%d.%d %d:%02d ", stt.wDay, stt.wMonth, stt.wYear, stt.wHour, stt.wMinute);
	std::cout << sn << " started;" << std::endl;
}
// Асинхронная функция завершения обслуживающего потока
void CALLBACK ASFinishMessage(DWORD Lprm) {
	Contact* client = (Contact*)Lprm;
	/*EnterCriticalSection(&scListContact);*/
	// Ставим метку срабатывания таймера
	char* sn = client->srvname;
	// Покидаем критическую секцию
	/*LeaveCriticalSection(&scListContact);*/
	
	SYSTEMTIME stt;
	GetLocalTime(&stt);
	printf("%d.%d.%d %d:%02d ", stt.wDay, stt.wMonth, stt.wYear, stt.wHour, stt.wMinute);
	std::cout << sn << " stoped;" << std::endl;
}

// Реализация функций для работы с библиотеками
bool LoadLibraryDLL(const char* libName) {
	if (libName == NULL || strlen(libName) == 0) {
		return false;
	}

	EnterCriticalSection(&scListLoadedLibraries);

	// Проверяем, не загружена ли уже эта библиотека
	for (ListLoadedLibraries::iterator it = LoadedLibraries.begin(); it != LoadedLibraries.end(); ++it) {
		if (strcmp(it->libName, libName) == 0) {
			LeaveCriticalSection(&scListLoadedLibraries);
			cout << "Library " << libName << " is already loaded" << endl;
			return false;
		}
	}


	char fullLibName[256];
	strcpy_s(fullLibName, libName);
	if (strstr(fullLibName, ".dll") == NULL && strstr(fullLibName, ".DLL") == NULL) {
		strcat_s(fullLibName, ".dll");
	}


	HMODULE hModule = LoadLibrary(fullLibName);
	if (hModule == NULL) {
		LeaveCriticalSection(&scListLoadedLibraries);
		cout << "Error loading library: " << libName << " (Error code: " << GetLastError() << ")" << endl;
		return false;
	}

	// Получаем адрес функции SSS из библиотеки
	HANDLE(*sssFunc)(char*, LPVOID) = (HANDLE(*)(char*, LPVOID))GetProcAddress(hModule, "SSS");
	if (sssFunc == NULL) {
		FreeLibrary(hModule);
		LeaveCriticalSection(&scListLoadedLibraries);
		cout << "Error: function SSS not found in library: " << libName << endl;
		return false;
	}

	// Добавляем библиотеку в список (сохраняем исходное имя без расширения для совместимости)
	LoadedLibrary lib;
	strcpy_s(lib.libName, libName);
	lib.hModule = hModule;
	lib.sssFunc = sssFunc;
	LoadedLibraries.push_back(lib);

	LeaveCriticalSection(&scListLoadedLibraries);
	cout << "Library loaded successfully: " << libName << " (file: " << fullLibName << ")" << endl;
	return true;
}

bool UnloadLibraryDLL(const char* libName) {
	if (libName == NULL || strlen(libName) == 0) {
		return false;
	}

	EnterCriticalSection(&scListLoadedLibraries);

	bool found = false;
	for (ListLoadedLibraries::iterator it = LoadedLibraries.begin(); it != LoadedLibraries.end(); ++it) {
		if (strcmp(it->libName, libName) == 0) {
			// Выгружаем библиотеку
			if (FreeLibrary(it->hModule)) {
				LoadedLibraries.erase(it);
				cout << "Library unloaded successfully: " << libName << endl;
				found = true;
			}
			else {
				cout << "Error unloading library: " << libName << " (Error code: " << GetLastError() << ")" << endl;
			}
			break;
		}
	}

	if (!found) {
		cout << "Library not found: " << libName << endl;
	}

	LeaveCriticalSection(&scListLoadedLibraries);
	return found;
}

HANDLE FindServiceInLibraries(char* serviceName, LPVOID prm) {
	if (serviceName == NULL || strlen(serviceName) == 0) {
		return NULL;
	}

	EnterCriticalSection(&scListLoadedLibraries);

	// Ищем сервис во всех загруженных библиотеках
	HANDLE result = NULL;
	for (ListLoadedLibraries::iterator it = LoadedLibraries.begin(); it != LoadedLibraries.end(); ++it) {
		if (it->sssFunc != NULL) {
			result = it->sssFunc(serviceName, prm);
			if (result != NULL) {
				// Сервис найден
				break;
			}
		}
	}

	LeaveCriticalSection(&scListLoadedLibraries);
	return result;
}