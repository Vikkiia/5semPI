#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Global.h"
#include <WS2tcpip.h>
using std::iterator;

DWORD WINAPI GarbageCleaner(LPVOID pPrm) {
	DWORD rc = 0;

	cout << "GarbageCleaner working\n";
	try {
		while (*((TalkersCommand*)pPrm) != EXIT || ClientServiceNumber > 0) {
			EnterCriticalSection(&scListContact);

			for (std::list<Contact>::iterator contact = Contacts.begin(); contact != Contacts.end();) {
				// При команде EXIT закрываем все соединения немедленно
				if (*((TalkersCommand*)pPrm) == EXIT && contact->sthread != Contact::FINISH && contact->sthread != Contact::TIMEOUT && contact->sthread != Contact::ABORT) {
					// Закрываем активное соединение при exit
					char closeMsg[50] = "Close: Abort;";
					send(contact->s, closeMsg, sizeof(closeMsg), NULL);
					contact->sthread = Contact::ABORT;
				}
				
				if (contact->sthread == Contact::FINISH || contact->sthread == Contact::TIMEOUT || contact->sthread == Contact::ABORT || contact->type == Contact::EMPTY) {
					char dst[16] = "";
					inet_ntop(AF_INET, &(contact->prms.sin_addr), dst, sizeof dst);

					printf_s("IP remote client: %s", dst);
					cout << " with code " << contact->sthread << ";" << endl;
					if (contact->type == Contact::EMPTY) InterlockedIncrement(&Fail);	//состояние сервера подключения
					else { 	// состояние обслуживающего сервера
						if (contact->sthread == Contact::FINISH)	{ InterlockedIncrement(&Finished); InterlockedDecrement(&Accept); }
						if (contact->sthread == Contact::TIMEOUT) { InterlockedIncrement(&Timeout); InterlockedDecrement(&Accept); }
						if (contact->sthread == Contact::ABORT)	{ InterlockedIncrement(&Fail); InterlockedDecrement(&Accept); }
						CloseHandle(contact->hthread);
						CloseHandle(contact->htimer);
					}
					closesocket(contact->s);
					contact = Contacts.erase(contact);
					InterlockedDecrement(&ClientServiceNumber);
				}
				else contact++;
			}

			LeaveCriticalSection(&scListContact);
			Sleep(1000);
		}
	}
	catch (string errorMsgText) {
		std::cout << errorMsgText << endl;
	}
	catch (...) {
		cout << "Error GarbageCleaner" << endl;
	}
	cout << "GarbageCleaner stopped\n" << endl;
	ExitThread(rc);
}