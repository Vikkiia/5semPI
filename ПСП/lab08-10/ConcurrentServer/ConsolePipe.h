#pragma once
#include "Global.h"

DWORD WINAPI ConsolePipe(LPVOID pPrm) {
	cout << "ConsolePipe working\n";
	DWORD rc = 0;
	HANDLE hPipe;
	try 
	{
		char rnpname[50];
		strcpy_s(rnpname, "\\\\.\\pipe\\");
		strcat_s(rnpname, npname);
		if ((hPipe = CreateNamedPipe(rnpname, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, NULL, NULL, INFINITE, NULL)) == INVALID_HANDLE_VALUE) throw SetErrorMsgText("Create:", GetLastError());

		while (*((TalkersCommand*)pPrm) != EXIT) {
			if (!ConnectNamedPipe(hPipe, NULL)) throw SetErrorMsgText("Connect:", GetLastError());
			char ReadBuf[50], WriteBuf[50];
			DWORD nBytesRead = 0, nBytesWrite = 0;
			TalkersCommand SetCommand;
			bool serverCommand;

			//циклически  проверяет общую область памяти потоков
			while (*((TalkersCommand*)pPrm) != EXIT) {
				//после получения команды  getcommand запрашивает следующую команду управления сервером
				if (*((TalkersCommand*)pPrm) == GETCOMMAND) {

					if (!ReadFile(hPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) break;
					if (nBytesRead > 0) {
					
						ReadBuf[nBytesRead] = '\0';
						
						serverCommand = true;
						
						
			
						char cmdUpper[50];
						strcpy_s(cmdUpper, ReadBuf);
						for (int i = 0; cmdUpper[i]; i++) {
							cmdUpper[i] = toupper(cmdUpper[i]);
						}
						
						
						if (strcmp(cmdUpper, "START") == 0) {
							sprintf_s(WriteBuf, "%s", "START");
							SetCommand = TalkersCommand::START;
						}
						else if (strcmp(cmdUpper, "STOP") == 0) {
							sprintf_s(WriteBuf, "%s", "STOP");
							SetCommand = TalkersCommand::STOP;
						}
						else if (strcmp(cmdUpper, "EXIT") == 0) {
							sprintf_s(WriteBuf, "%s", "EXIT");
							SetCommand = TalkersCommand::EXIT;
						}
						else if (strcmp(cmdUpper, "STATISTICS") == 0 || strcmp(cmdUpper, "STATS") == 0) {
							sprintf_s(WriteBuf, "\nActive: \t%i\nBounces: \t%i\nClosed: \t%i\nTimeout: \t%i\n", Accept, Fail, Finished, Timeout);
							serverCommand = false;
						}
						else if (strcmp(cmdUpper, "WAIT") == 0) {
							sprintf_s(WriteBuf, "%s", "WAIT");
							SetCommand = TalkersCommand::WAIT;
						}
						else if (strcmp(cmdUpper, "SHUTDOWN") == 0) {
							sprintf_s(WriteBuf, "%s", "SHUTDOWN");
							SetCommand = TalkersCommand::SHUTDOWN;
						}
						
						else if (strncmp(ReadBuf, "LOAD_LIB ", 9) == 0) {
							// Извлекаем имя библиотеки (после "LOAD_LIB ")
							char libName[256];
							strcpy_s(libName, ReadBuf + 9);
							
							// Убираем возможные пробелы в начале и конце
							while (libName[0] == ' ' && strlen(libName) > 0) {
								memmove(libName, libName + 1, strlen(libName));
							}
							int len = strlen(libName);
							while (len > 0 && libName[len - 1] == ' ') {
								libName[len - 1] = '\0';
								len--;
							}
							
							if (strlen(libName) > 0) {
								if (LoadLibraryDLL(libName)) {
									sprintf_s(WriteBuf, "Library loaded: %s", libName);
								}
								else {
									sprintf_s(WriteBuf, "Error loading library: %s", libName);
								}
								serverCommand = false;
							}
							else {
								sprintf_s(WriteBuf, "Error: library name is empty");
								serverCommand = false;
							}
						}
						else if (strncmp(ReadBuf, "UNLOAD_LIB ", 11) == 0) {
							// Извлекаем имя библиотеки (после "UNLOAD_LIB ")
							char libName[256];
							strcpy_s(libName, ReadBuf + 11);
							
							// Убираем возможные пробелы в начале и конце
							while (libName[0] == ' ' && strlen(libName) > 0) {
								memmove(libName, libName + 1, strlen(libName));
							}
							int len = strlen(libName);
							while (len > 0 && libName[len - 1] == ' ') {
								libName[len - 1] = '\0';
								len--;
							}
							
							if (strlen(libName) > 0) {
								if (UnloadLibraryDLL(libName)) {
									sprintf_s(WriteBuf, "Library unloaded: %s", libName);
								}
								else {
									sprintf_s(WriteBuf, "Error unloading library: %s", libName);
								}
								serverCommand = false;
							}
							else {
								sprintf_s(WriteBuf, "Error: library name is empty");
								serverCommand = false;
							}
						}
						else {
							// Обработка числовых команд (для совместимости со старым форматом)
							int n = atoi(ReadBuf);
							
							switch (n) {
								case 0:
									sprintf_s(WriteBuf, "%s", "START");
									SetCommand = TalkersCommand::START;
									break;
								case 1:
									sprintf_s(WriteBuf, "%s", "STOP");
									SetCommand = TalkersCommand::STOP;
									break;
								case 2:
									sprintf_s(WriteBuf, "%s", "EXIT");
									SetCommand = TalkersCommand::EXIT;
									break;
								case 3:
									sprintf_s(WriteBuf, "\nActive: \t%i\nBounces: \t%i\nClosed: \t%i\nTimeout: \t%i\n", Accept, Fail, Finished, Timeout);
									serverCommand = false;
									break;
								case 4:
									sprintf_s(WriteBuf, "%s", "WAIT");
									SetCommand = TalkersCommand::WAIT;
									break;
								case 5:
									sprintf_s(WriteBuf, "%s", "SHUTDOWN");
									SetCommand = TalkersCommand::SHUTDOWN;
									break;
								default:
									sprintf_s(WriteBuf, "%s", "nocmd");
									serverCommand = false;
									break;
							}
						}
						
						if (serverCommand == true) {
							*((TalkersCommand*)pPrm) = SetCommand;
							printf_s("ConsolePipe: command %s\n", WriteBuf);
						}
						if (!WriteFile(hPipe, WriteBuf, sizeof(WriteBuf), &nBytesWrite, NULL)) throw new string("CP WRITE ERROR");
					}
				}
				else Sleep(1000);
			}
			if (!DisconnectNamedPipe(hPipe)) throw SetErrorMsgText("disconnect:", GetLastError());
		}
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
		cout << "ConsolePipe stopped" << endl;
	}
	catch (string ErrorPipeText) {
		cout << ErrorPipeText << endl;
	}
	catch (...) {
		cout << "Error ConsolePipe" << endl;
	}
	ExitThread(rc);
}