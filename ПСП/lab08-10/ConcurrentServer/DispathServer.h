#pragma once
#include "Global.h"


DWORD WINAPI DispathServer(LPVOID pPrm) {
	cout << "DispathServer working\n";
	DWORD rc = 0;
	try {
		while (*((TalkersCommand*)pPrm) != EXIT) {
			try {
				if (WaitForSingleObject(Event, 300) == WAIT_OBJECT_0) {

					if (&ClientServiceNumber > (volatile LONG*)0) {
						Contact* client = NULL;
						int libuf = 1;
						char CallBuf[50] = "", SendError[50] = "ErrorInquiry";
						EnterCriticalSection(&scListContact);

						//должен последовательно просматривать список ListContact  (в котором хранятся все сведения о подключившихся клиентах)
						//и считывать запрос, подключившегося клиента
						for (ListContact::iterator p = Contacts.begin(); p != Contacts.end(); p++) {
							if (p->type == Contact::ACCEPT && p->sthread != Contact::ABORT)
							{
								client = &(*p);

								//При обнаружении в списке подключившегося, но не обслуженного клиента (это можно определить  по специальным признакам, 
								//записанным в элемент списка) следует прочитать первое сообщение клиента (которое должно быть кодом запроса) с помощью
								//функции recv (раздел 3.11).
								bool flag = false;
								while (flag == false) {
									if ((libuf = recv(client->s, CallBuf, sizeof(CallBuf), NULL)) == SOCKET_ERROR) {
										switch (WSAGetLastError()) {
											case WSAEWOULDBLOCK: Sleep(100); break;
											default: throw  SetErrorMsgText("Recv:", WSAGetLastError());
										}
									}
									else flag = true;
								}

								// Пытаемся найти сервис в загруженных библиотеках (поддержка любых сервисов)
								if (flag == true && strlen(CallBuf) > 0) {
									client->type = Contact::CONTACT;
									strcpy_s(client->srvname, CallBuf);

									client->htimer = CreateWaitableTimer(NULL, false, NULL); // true - ручной сброс, false - автоматический
									
									_int64 time = -1800000000LL; 
									SetWaitableTimer(client->htimer, (LARGE_INTEGER*)&time, 0, ASWTimer, client, false); // 0 - не периодическая
									
									
									client->hthread = FindServiceInLibraries(CallBuf, client);
									
									if (client->hthread != NULL) {
										// Сервис найден
										cout << CallBuf << " server called" << endl;
										if ((libuf = send(client->s, CallBuf, sizeof(CallBuf), NULL)) == SOCKET_ERROR) throw SetErrorMsgText("Send:", WSAGetLastError());
									}
									else {
										
										char errorMsg[50] = "ErrorInquiry";
										if ((libuf = send(client->s, errorMsg, sizeof(errorMsg) + 1, NULL)) == SOCKET_ERROR) throw SetErrorMsgText("Send:", WSAGetLastError());
										closesocket(client->s);
										client->sthread = Contact::ABORT;
										CancelWaitableTimer(client->htimer);
										InterlockedIncrement(&Fail);
										cout << "Service " << CallBuf << " not found or all libraries unloaded" << endl;
									}
								}
								else {
								//обслуживающий сервер, завершает свою работу до истечения  
								//установленного максимального интервала его работы
									if ((libuf = send(client->s, SendError, sizeof(SendError) + 1, NULL)) == SOCKET_ERROR) throw SetErrorMsgText("Send:", WSAGetLastError());
									closesocket(client->s);
									client->sthread = Contact::ABORT;
									CancelWaitableTimer(client->htimer);
									InterlockedIncrement(&Fail);
								}
							}
						}
						LeaveCriticalSection(&scListContact);
					}
					SleepEx(0, true);
				}
				SleepEx(0, true);
			}
			catch (string errorMsgText) {
				std::cout << errorMsgText << endl;
			}
		}
	}
	catch (string errorMsgText) {
		std::cout << errorMsgText << endl;
	}
	catch (...) {
		std::cout << "Error DispathServer" << std::endl;
	}
	cout << "DispathServer stopped\n" << endl;
	ExitThread(rc);
}