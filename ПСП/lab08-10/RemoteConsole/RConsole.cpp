#include <stdio.h>
#include <tchar.h>
#include "iostream"
#include "Windows.h"
#include "ctime"
#include <string>
#include <cstring>
#include <cctype>

using std::string;
using namespace std;

string GetErrorMsgText(int code) 
{
	char buff[50];
	DWORD bufflen = sizeof(buff);
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return std::string(); //No error message has been recorded
	}
	else {
		return std::system_category().message(errorMessageID);
	}
};
string SetPipeError(string msgText, int code)
{
	return msgText + to_string(code) + ". " + GetErrorMsgText(code);
};


int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "Russian");

	char ReadBuf[512] = "";
	char WriteBuf[512] = "";
	char CommandBuf[512] = "";
	DWORD nBytesRead;
	DWORD nBytesWrite;

#pragma region SecurityAttributes
	SECURITY_DESCRIPTOR* m_pSecDesc = (SECURITY_DESCRIPTOR*)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	SECURITY_ATTRIBUTES m_pSecAttrib = { sizeof(SECURITY_ATTRIBUTES), m_pSecDesc, TRUE };
	InitializeSecurityDescriptor(m_pSecDesc, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(m_pSecDesc, TRUE, (PACL)NULL, FALSE);
#pragma endregion


	int Code = 0;

	char serverName[256];
	char PipeName[512];
	bool result;

	try
	{
		printf_s("\n ---------- Available Commands ---------- \n");
		printf_s("1 or 'start'  \t (allow client connections to the server)\n");
		printf_s("2 or 'stop'  \t (deny client connections to the server)\n");
		printf_s("3 or 'exit'  \t (terminate the server)\n");
		printf_s("4 or 'statistics' (display statistics)\n");
		printf_s("5 or 'wait'  \t (pause client connections)\n");
		printf_s("6 or 'shutdown'  (wait + exit)\n");
		/*printf_s("LOAD_LIB <name>  (load DLL library)\n");
		printf_s("UNLOAD_LIB <name>  (unload DLL library)\n");*/
		printf_s("0 - close\n");
		printf_s("\n ---------- ---------- ---------- ---------- \n");

		cout << "Enter server name: ";
		cin >> serverName;
		cin.ignore(1000, '\n'); // Очищаем буфер после ввода имени сервера
		result = sprintf_s(PipeName, "\\\\%s\\pipe\\cpipe", serverName);


		HANDLE hNamedPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, &m_pSecAttrib);

		do {
			printf_s("Command: ");
			
			cin.getline(CommandBuf, sizeof(CommandBuf));

			// Проверяем, является ли это числовой командой
			if (strlen(CommandBuf) > 0) {
				// Удаляем пробелы в начале и конце
				char* start = CommandBuf;
				while (*start == ' ') start++;
				char* end = CommandBuf + strlen(CommandBuf) - 1;
				while (end > start && (*end == ' ' || *end == '\n' || *end == '\r')) end--;
				*(end + 1) = '\0';

				char* endptr;
				long numCommand = strtol(start, &endptr, 10);

				// Если это число (endptr указывает на конец строки)
				if (*endptr == '\0' || *endptr == '\n' || *endptr == '\r') {
					Code = (int)numCommand;
					if (Code > 0 && Code < 7) {
						sprintf_s(WriteBuf, "%d", Code - 1);
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
						// Если команда exit или shutdown, выходим из цикла
						if (Code == 3 || Code == 6) {
							break;
						}
					}
					if (Code == 0) break;
				}
				
				else {
					// Преобразуем в верхний регистр для сравнения
					char cmdUpper[512];
					strcpy_s(cmdUpper, start);
					for (int i = 0; cmdUpper[i]; i++) {
						cmdUpper[i] = (char)toupper((unsigned char)cmdUpper[i]);
					}

					if (strcmp(cmdUpper, "START") == 0) {
						strcpy_s(WriteBuf, "START");
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
					}
					else if (strcmp(cmdUpper, "STOP") == 0) {
						strcpy_s(WriteBuf, "STOP");
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
					}
					else if (strcmp(cmdUpper, "EXIT") == 0) {
						strcpy_s(WriteBuf, "EXIT");
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
						break; // Выходим из цикла после exit
					}
					else if (strcmp(cmdUpper, "STATISTICS") == 0 || strcmp(cmdUpper, "STATS") == 0) {
						strcpy_s(WriteBuf, "STATISTICS");
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
					}
					else if (strcmp(cmdUpper, "WAIT") == 0) {
						strcpy_s(WriteBuf, "WAIT");
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
					}
					else if (strcmp(cmdUpper, "SHUTDOWN") == 0) {
						strcpy_s(WriteBuf, "SHUTDOWN");
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
						break; // Выходим из цикла после shutdown
					}
					// Если это текстовая команда LOAD_LIB или UNLOAD_LIB
					else if (strncmp(start, "LOAD_LIB ", 9) == 0 || strncmp(start, "load_lib ", 9) == 0) {
						// Отправляем команду как есть
						strcpy_s(WriteBuf, start);
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
					}
					else if (strncmp(start, "UNLOAD_LIB ", 11) == 0 || strncmp(start, "unload_lib ", 11) == 0) {
						// Отправляем команду как есть
						strcpy_s(WriteBuf, start);
						if (!WriteFile(hNamedPipe, WriteBuf, (DWORD)strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw string("WriteFile: Error ");
						if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw string("ReadFile: Error ");
						cout << ReadBuf << endl;
					}
					else {
						cout << "Unknown command. Use numbers 0-6 or LOAD_LIB/UNLOAD_LIB commands." << endl;
					}
				}
			}
		} while (true);

		if (!CloseHandle(hNamedPipe)) throw SetPipeError("CloseHandle: ", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
	cout << "RConsole stopped\n\n";
	return 0;
}
