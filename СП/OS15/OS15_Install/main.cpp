#include <Windows.h>
#include <iostream>

#define SERVICENAME L"OS15_HTService"

#ifdef _WIN64
#define SERVICEPATH L"D:\laby\5_sem\SP\6_lab\OS15\\x64\\Debug\\OS15_HTService.exe"
#else
#define SERVICEPATH L"D:\laby\5_sem\SP\6_lab\OS15\\Debug\\OS15_HTService.exe"
#endif

char* errortxt(const char* msg, int code)
{
	char* buf = new char[512];

	sprintf_s(buf, 512, "%s: error code = %d\n", msg, code);

	return buf;
}

int main()
{
	SC_HANDLE schService = NULL, schSCManager = NULL;
	try
	{
		schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

		if (!schSCManager)
		{
			throw errortxt("OpenSCManager", GetLastError());
		}
		else
		{
			schService = CreateService(
				schSCManager,
				SERVICENAME,
				SERVICENAME,
				SERVICE_ALL_ACCESS,
				SERVICE_WIN32_SHARE_PROCESS,
				SERVICE_AUTO_START,
				SERVICE_ERROR_NORMAL,
				SERVICEPATH,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
			);	

			if (!schService)
			{
				throw errortxt("CreateService", GetLastError());
			}
		}
	}
	catch (char* txt)
	{
		std::cout << txt << std::endl;
	}


	if (schSCManager)
	{
		CloseServiceHandle(schSCManager);
	}

	if (schService)
	{
		CloseServiceHandle(schService);
	}

	return 0;
}