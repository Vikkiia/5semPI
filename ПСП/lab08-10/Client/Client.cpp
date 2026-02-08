#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>
#include "iostream"
#include "Winsock2.h"                
#pragma comment(lib, "WS2_32.lib")   
#include <string>
#include <ctime>
#include "time.h"

using namespace std;

// Функция для вычисления broadcast адреса по IP и маске
unsigned long CalculateBroadcast(unsigned long ip, unsigned long mask) {
	return (ip & mask) | (~mask);
}

// Функция для получения локального IP адреса и вычисления broadcast
// Пробует несколько распространенных масок подсети
bool GetLocalBroadcastAddress(char* broadcastAddr, int size) {
	SOCKET testSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (testSocket == INVALID_SOCKET) {
		return false;
	}

	SOCKADDR_IN testAddr;
	testAddr.sin_family = AF_INET;
	testAddr.sin_port = htons(2000);
	testAddr.sin_addr.s_addr = inet_addr("8.8.8.8"); // Google DNS для теста

	// Пытаемся подключиться (не обязательно успешно)
	connect(testSocket, (sockaddr*)&testAddr, sizeof(testAddr));

	// Получаем локальный адрес сокета
	SOCKADDR_IN localAddr;
	int len = sizeof(localAddr);
	if (getsockname(testSocket, (sockaddr*)&localAddr, &len) == 0) {
		closesocket(testSocket);

		unsigned long ip = ntohl(localAddr.sin_addr.s_addr);

		// Пробуем несколько распространенных масок
		unsigned long masks[] = {
			0xFFFFFF00,
			0xFFFFFFF0,
			0xFFFFFFE0,
			0xFFFFFFF8,
			0xFFFF0000,
			0xFF000000
		};

		for (int i = 0; i < 6; i++) {
			unsigned long broadcast = CalculateBroadcast(ip, masks[i]);
			struct in_addr addr;
			addr.s_addr = htonl(broadcast);
			char* ipStr = inet_ntoa(addr);
			if (ipStr != NULL) {
				strcpy_s(broadcastAddr, size, ipStr);
				return true;
			}
		}
	}

	closesocket(testSocket);
	return false;
}

bool SystemMessage(char* ch) {

	bool result = false;
	try {

		char Timeout[50] = "Close: timeout;", Close[50] = "Close: finish;", Abort[50] = "Close: Abort;";
		if (strcmp(ch, Timeout) == 0) result = true;
		else if (strcmp(ch, Abort) == 0) result = true;
		else if (strcmp(ch, Close) == 0) result = true;
	}
	catch (...) {
		cout << "err" << endl;
	}

	return result;
}

string  GetErrorMsgText(int code) {
	string msgText;
	switch (code) {
	case WSAEINTR:                  msgText = "WSAEINTR"; break;
	case WSAEACCES:                 msgText = "WSAEACCES"; break;
	case WSAEFAULT:                 msgText = "WSAEFAULT"; break;
	case WSAEINVAL:                 msgText = "WSAEINVAL"; break;
	case WSAEMFILE:                 msgText = "WSAEMFILE"; break;
	case WSAEWOULDBLOCK:            msgText = "WSAEWOULDBLOCK"; break;
	case WSAEINPROGRESS:            msgText = "WSAEINPROGRESS"; break;
	case WSAEALREADY:               msgText = "WSAEALREADY"; break;
	case WSAENOTSOCK:               msgText = "WSAENOTSOCK"; break;
	case WSAEDESTADDRREQ:           msgText = "WSAEDESTADDRREQ"; break;
	case WSAEMSGSIZE:               msgText = "WSAEMSGSIZE"; break;
	case WSAEPROTOTYPE:             msgText = "WSAEPROTOTYPE"; break;
	case WSAENOPROTOOPT:            msgText = "WSAENOPROTOOPT"; break;
	case WSAEPROTONOSUPPORT:        msgText = "WSAEPROTONOSUPPORT"; break;
	case WSAESOCKTNOSUPPORT:        msgText = "WSAESOCKTNOSUPPORT"; break;
	case WSAEOPNOTSUPP:             msgText = "WSAEOPNOTSUPP"; break;
	case WSAEPFNOSUPPORT:           msgText = "WSAEPFNOSUPPORT"; break;
	case WSAEAFNOSUPPORT:           msgText = "WSAEAFNOSUPPORT"; break;
	case WSAEADDRINUSE:             msgText = "WSAEADDRINUSE"; break;
	case WSAEADDRNOTAVAIL:          msgText = "WSAEADDRNOTAVAIL"; break;
	case WSAENETDOWN:               msgText = "WSAENETDOWN"; break;
	case WSAENETUNREACH:            msgText = "WSAENETUNREACH"; break;
	case WSAENETRESET:              msgText = "WSAENETRESET"; break;
	case WSAECONNABORTED:           msgText = "WSAECONNABORTED"; break;
	case WSAECONNRESET:             msgText = "WSAECONNRESET"; break;
	case WSAENOBUFS:                msgText = "WSAENOBUFS"; break;
	case WSAEISCONN:                msgText = "WSAEISCONN"; break;
	case WSAENOTCONN:               msgText = "WSAENOTCONN"; break;
	case WSAESHUTDOWN:              msgText = "WSAESHUTDOWN"; break;
	case WSAETIMEDOUT:              msgText = "WSAETIMEDOUT"; break;
	case WSAECONNREFUSED:           msgText = "WSAECONNREFUSED"; break;
	case WSAEHOSTDOWN:              msgText = "WSAEHOSTDOWN"; break;
	case WSAEHOSTUNREACH:           msgText = "WSAEHOSTUNREACH"; break;
	case WSAEPROCLIM:               msgText = "WSAEPROCLIM"; break;
	case WSASYSNOTREADY:            msgText = "WSASYSNOTREADY"; break;
	case WSAVERNOTSUPPORTED:        msgText = "WSAVERNOTSUPPORTED"; break;
	case WSANOTINITIALISED:         msgText = "WSANOTINITIALISED"; break;
	case WSAEDISCON:                msgText = "WSAEDISCON"; break;
	case WSATYPE_NOT_FOUND:         msgText = "WSATYPE_NOT_FOUND"; break;
	case WSAHOST_NOT_FOUND:         msgText = "WSAHOST_NOT_FOUND"; break;
	case WSATRY_AGAIN:              msgText = "WSATRY_AGAIN"; break;
	case WSANO_RECOVERY:            msgText = "WSANO_RECOVERY"; break;
	case WSANO_DATA:                msgText = "WSANO_DATA"; break;
	case WSA_INVALID_HANDLE:        msgText = "WSA_INVALID_HANDLE"; break;
	case WSA_INVALID_PARAMETER:     msgText = "WSA_INVALID_PARAMETER"; break;
	case WSA_IO_INCOMPLETE:         msgText = "WSA_IO_INCOMPLETE"; break;
	case WSA_IO_PENDING:            msgText = "WSA_IO_PENDING"; break;
	case WSA_NOT_ENOUGH_MEMORY:     msgText = "WSA_NOT_ENOUGH_MEMORY"; break;
	case WSA_OPERATION_ABORTED:     msgText = "WSA_OPERATION_ABORTED"; break;
	case WSASYSCALLFAILURE:         msgText = "WSASYSCALLFAILURE"; break;
	default:                        msgText = "***ERROR***"; break;
	};
	return msgText;
}

string  SetErrorMsgText(string msgText, int code) {
	return  msgText + GetErrorMsgText(code);
}

string error = "Error";

string GetError() {
	return error;
}

bool GetServer(char* call, SOCKADDR_IN* from, int* flen, SOCKET* cC, SOCKADDR_IN* all) {
	char ibuf[50];
	int  libuf = 0, lobuf = 0;

	try {
		// Отправка запроса с позывным
		if ((lobuf = sendto(*cC, call, strlen(call) + 1, NULL, (sockaddr*)all, sizeof(*all))) == SOCKET_ERROR) {
			int error = WSAGetLastError();
			throw SetErrorMsgText("Sendto:", error);
		}

		// Ожидание ответа от сервера
		memset(ibuf, 0, sizeof(ibuf));
		if ((libuf = recvfrom(*cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)from, flen)) == SOCKET_ERROR) {
			int error = WSAGetLastError();
			if (error == WSAETIMEDOUT) {
				return false;
			}
			else {
				throw SetErrorMsgText("Recvfrom:", error);
			}
		}

		// Проверка совпадения позывного
		if (libuf > 0 && strcmp(call, ibuf) == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	catch (string errorMsg) {
		throw errorMsg;
	}
	catch (...) {
		throw string("Unknown error in GetServer");
	}
}

int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "Russian");
	int port = 0;
	SOCKET  SocketTCP;
	WSADATA wsaData;


	try {
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) throw SetErrorMsgText("Startup:", WSAGetLastError());
		if ((SocketTCP = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) throw  SetErrorMsgText("Socket:", WSAGetLastError());

		int ch = 0, cCall = 0;

		int max = 100, lobuf = 1, bport = 2000;
		char obuf[50] = "", ibuf[50] = "", Call[50], Name[50], call[50];

		SOCKADDR_IN Server = { 0 };
		Server.sin_family = AF_INET;
		Server.sin_port = htons(port);

		SOCKADDR_IN Server_IN;
		int Flen = sizeof(Server), connectionType = 0;

		cout << "1 - Connect by server name" << endl;
		cout << "2 - Connect by call sign" << endl;
		cin >> connectionType;

		if (connectionType == 1) {
			cout << "Enter server name: ";
			cin >> Name;
			hostent* s = gethostbyname(Name);
			if (s == NULL) throw "Server not found;";
			cout << "Enter server port: ";
			cin >> port;
			Server_IN.sin_addr = *(struct in_addr*)s->h_addr_list[0];
		}
		else if (connectionType == 2) {
			cout << "Enter server call sign: ";
			cin >> call;

			SOCKET SocketUDP = INVALID_SOCKET;
			try {
				int optval = 1;
				if ((SocketUDP = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
					throw SetErrorMsgText("Socket:", WSAGetLastError());
				}
				if (setsockopt(SocketUDP, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR) {
					throw SetErrorMsgText("Setsockopt SO_BROADCAST:", WSAGetLastError());
				}

				DWORD timeout = 2000;
				if (setsockopt(SocketUDP, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
					throw SetErrorMsgText("Setsockopt SO_RCVTIMEO:", WSAGetLastError());
				}

				SOCKADDR_IN all;
				all.sin_family = AF_INET;
				all.sin_port = htons(bport);

				SOCKADDR_IN from;
				memset(&from, 0, sizeof(from));
				int lc = sizeof(from);

				cout << "Searching for server with call sign \"" << call << "\"..." << endl;
				bool bsr = false;

				char autoBroadcast[16] = "";
				if (GetLocalBroadcastAddress(autoBroadcast, sizeof(autoBroadcast))) {
					cout << "Attempt 1: Auto-detected broadcast " << autoBroadcast << "..." << endl;
					all.sin_addr.s_addr = inet_addr(autoBroadcast);
					if (all.sin_addr.s_addr != INADDR_NONE) {
						try {
							bsr = GetServer(call, &from, &lc, &SocketUDP, &all);
						}
						catch (...) {
							bsr = false;
						}
					}
				}

				if (!bsr) {
					cout << "Attempt 2: Broadcast 255.255.255.255..." << endl;
					all.sin_addr.s_addr = inet_addr("255.255.255.255");
					if (all.sin_addr.s_addr == INADDR_NONE) {
						all.sin_addr.s_addr = 0xFFFFFFFF;
					}
					try {
						bsr = GetServer(call, &from, &lc, &SocketUDP, &all);
					}
					catch (...) {
						bsr = false;
					}
				}

				if (!bsr) {
					cout << "Attempt 3: Localhost 127.0.0.1..." << endl;
					all.sin_addr.s_addr = inet_addr("127.0.0.1");
					try {
						bsr = GetServer(call, &from, &lc, &SocketUDP, &all);
					}
					catch (...) {
						bsr = false;
					}
				}

				if (!bsr) {
					const char* broadcastAddrs[] = {
						"172.20.10.15",
						"172.20.10.255",
						"192.168.1.255",
						"192.168.0.255",
						"10.0.0.255"
					};

					for (int i = 0; i < 5 && !bsr; i++) {
						cout << "Attempt " << (4 + i) << ": Broadcast " << broadcastAddrs[i] << "..." << endl;
						all.sin_addr.s_addr = inet_addr(broadcastAddrs[i]);
						if (all.sin_addr.s_addr != INADDR_NONE) {
							try {
								bsr = GetServer(call, &from, &lc, &SocketUDP, &all);
							}
							catch (...) {
								bsr = false;
							}
						}
					}
				}

				if (!bsr) {
					cout << endl << "========================================" << endl;
					cout << "Server not found after all attempts." << endl;
					cout << "========================================" << endl << endl;

					char localBroadcast[16] = "";
					if (GetLocalBroadcastAddress(localBroadcast, sizeof(localBroadcast))) {
						cout << "Your auto-detected broadcast: " << localBroadcast << endl;
					}

					cout << endl << "Check on SERVER:" << endl;
					cout << "1. Server is running and shows 'ResponseServer working'" << endl;
					cout << "2. UDP port " << bport << " is not blocked by firewall" << endl;
					cout << "3. Server call sign: " << call << " (must match exactly)" << endl;

					cout << endl << "Check on CLIENT:" << endl;
					cout << "1. Firewall allows outgoing UDP traffic on port " << bport << endl;
					cout << "2. Client and server are in same network (check with ipconfig)" << endl;

					cout << endl << "SOLUTION:" << endl;
					cout << "If broadcast does not work, connect by IP:" << endl;
					cout << "1. Get server IP (run ipconfig on server)" << endl;
					cout << "2. Select option 1 at client start" << endl;
					cout << "3. Enter server IP" << endl;
					cout << "========================================" << endl;
					throw string("Server not found");
				}

				char* serverIP = inet_ntoa(from.sin_addr);
				if (serverIP != NULL) {
					cout << "Server found at: " << serverIP << endl;
				}
				else {
					cout << "Server found" << endl;
				}
				Server_IN.sin_addr.s_addr = from.sin_addr.s_addr;
				cout << "Enter server port: ";
				cin >> port;
			}
			catch (string errorMsg) {
				if (SocketUDP != INVALID_SOCKET) {
					closesocket(SocketUDP);
				}
				throw errorMsg;
			}
			catch (...) {
				if (SocketUDP != INVALID_SOCKET) {
					closesocket(SocketUDP);
				}
				throw string("Unknown error while call-sign connect");
			}
		}
		else {
			throw "Wrong code";
		}

		Server_IN.sin_family = AF_INET;
		Server_IN.sin_port = htons(port);

		if ((connect(SocketTCP, (sockaddr*)&Server_IN, sizeof(Server_IN))) == SOCKET_ERROR)
			throw "Connection error;";

		printf_s("\n ---------- ---------- ---------- ---------- \n");
		cout << endl << "1 - Rand" << endl << "2 - Time" << endl << "3 - Echo" << endl;
		cin >> cCall;
		switch (cCall) {
		case 1:
			strcpy_s(Call, "Rand");
			break;
		case 2:
			strcpy_s(Call, "Time");
			break;
		case 3:
			strcpy_s(Call, "Echo");
			break;
		default:
			if (closesocket(SocketTCP) == SOCKET_ERROR)
				throw SetErrorMsgText("Closesocket:", WSAGetLastError());
			throw "Invalid code entered;";
		}
		printf_s("\n ---------- ---------- ---------- ---------- \n");


		if ((lobuf = send(SocketTCP, Call, sizeof(Call), NULL)) == SOCKET_ERROR)
			throw SetErrorMsgText("Send:", WSAGetLastError());

		char rCall[50];
		if ((lobuf = recv(SocketTCP, rCall, sizeof(rCall), NULL)) == SOCKET_ERROR)
			throw SetErrorMsgText("Recv:", WSAGetLastError());

		if (strcmp(Call, rCall) != 0) {
			throw "Service not available";
		}
		else {
			Sleep(100);

			u_long nonblk = 1;
			if (SOCKET_ERROR == ioctlsocket(SocketTCP, FIONBIO, &nonblk))
				throw SetErrorMsgText("Ioctlsocket:", WSAGetLastError());

			clock_t StartTime = clock();
			char command[50];
			bool sessionActive = true;

			cout << "Enter message for service:" << endl;

			try {
				while (sessionActive) {
					// Очистка буфера перед вводом
					memset(command, 0, sizeof(command));
					cin >> command;

					// Проверка на пустую команду
					if (strlen(command) == 0) {
						continue;
					}

					// Проверка команды exit - завершаемся сразу, не отправляя на сервер
					if (strcmp(command, "exit") == 0 || strcmp(command, "Exit") == 0 || strcmp(command, "EXIT") == 0) {
						error = "Client exit";
						sessionActive = false;
						break; // Выходим из цикла немедленно
					}

					if ((lobuf = send(SocketTCP, command, sizeof(command), NULL)) == SOCKET_ERROR) {
						throw "Error send";
					}

					bool awaitingResponse = true;
					int timeoutCounter = 0;
					const int maxTimeout = 10; // 30 секунд максимум ожидания (300 * 100ms)

					// Очистка буфера перед получением
					memset(obuf, 0, sizeof(obuf));

					while (awaitingResponse && timeoutCounter < maxTimeout) {
						int recvResult = recv(SocketTCP, obuf, sizeof(obuf), NULL);
						if (recvResult == SOCKET_ERROR) {
							switch (WSAGetLastError()) {
							case WSAEWOULDBLOCK:
								Sleep(100);
								timeoutCounter++;
								break;
							case WSAECONNRESET:
							case WSAECONNABORTED:
								// Соединение закрыто сервером
								printf("Server closed the connection unexpectedly\n");
								sessionActive = false;
								error = "Server closed connection";
								awaitingResponse = false;
								break;
							default: throw SetErrorMsgText("Recv:", WSAGetLastError());
							}
						}
						else if (recvResult == 0) {
							// Соединение закрыто сервером (graceful shutdown)
							printf("Server closed the connection\n");
							sessionActive = false;
							error = "Server closed connection";
							awaitingResponse = false;
						}
						else if (recvResult > 0) {
							if (SystemMessage(obuf)) {
								printf("Server closed the connection: %s\n", obuf);
								sessionActive = false;
								// Устанавливаем флаг ошибки для завершения с ненулевым кодом
								error = "Server closed connection";
							}
							else {
								printf("Received message: [%s]\n", obuf);
								if (strcmp(command, "exit") == 0 || strcmp(command, "Exit") == 0) {
									sessionActive = false;
								}
								else {
									cout << "Enter a message for the service (type 'exit' to finish):" << endl;
								}
							}
							awaitingResponse = false;
						}
					}

					if (timeoutCounter >= maxTimeout) {
						cout << "Server response timeout exceeded" << endl;
						sessionActive = false;
					}
				}
			}
			catch (string errorMsgText) {
				cout << "Error: " << errorMsgText << endl;
			}
			catch (char* errorMsgText) {
				cout << "Error: " << errorMsgText << endl;
			}
			catch (...) {
				cout << "Unknown error" << endl;
			}

			// Выводим время только если сессия была активной (не был exit)
			if (error != "Client exit") {
				clock_t FinishTime = clock();
				printf("Elapsed time: %lf sec.\n", (double)(FinishTime - StartTime) / CLOCKS_PER_SEC);
			}

		}
		if (closesocket(SocketTCP) == SOCKET_ERROR) {
			if (error != "Client exit") { // Не выводим ошибку при нормальном выходе клиента
				throw SetErrorMsgText("Closesocket:", WSAGetLastError());
			}
		}
		if (WSACleanup() == SOCKET_ERROR) {
			if (error != "Client exit") { // Не выводим ошибку при нормальном выходе клиента
				throw SetErrorMsgText("Cleanup:", WSAGetLastError());
			}
		}
}
catch (string errorMsgText) {
	cout << "Error: " << errorMsgText << endl;
	if (error == "Server closed connection" || error == "Client exit") {
		system("pause");
		return 1; // Возвращаем код ошибки при закрытии соединения сервером или выходе клиента
	}
}
catch (char* errorMsgText) {
	cout << "Error: " << errorMsgText << endl;
}
catch (...) {
	cout << "Unknown error" << endl;
}

// Проверяем, была ли ошибка закрытия соединения сервером или выход клиента
if (error == "Server closed connection" || error == "Client exit") {
	system("pause");
	return 1; // Возвращаем код ошибки при закрытии соединения сервером или выходе клиента
}

system("pause");
return 0;

}