#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include"WinSock2.h"
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "WS2_32.lib")

#define DISCOVERY_PORT  "2000"


using namespace std;

string GetErrorMessage() {
    int code = WSAGetLastError();
    string msg;

    switch (code)
    {
    case WSAEINTR:            msg = "WSAEINTR - Interrupted function call"; break;
    case WSAEBADF:            msg = "WSAEBADF - Bad file number"; break;
    case WSAEACCES:           msg = "WSAEACCES - Permission denied"; break;
    case WSAEFAULT:           msg = "WSAEFAULT - Bad address"; break;
    case WSAEINVAL:           msg = "WSAEINVAL - Invalid argument"; break;
    case WSAEMFILE:           msg = "WSAEMFILE - Too many open files"; break;
    case WSAEWOULDBLOCK:      msg = "WSAEWOULDBLOCK - Operation would block"; break;
    case WSAEINPROGRESS:      msg = "WSAEINPROGRESS - Operation now in progress"; break;
    case WSAEALREADY:         msg = "WSAEALREADY - Operation already in progress"; break;
    case WSAENOTSOCK:         msg = "WSAENOTSOCK - Socket operation on non-socket"; break;
    case WSAEDESTADDRREQ:     msg = "WSAEDESTADDRREQ - Destination address required"; break;
    case WSAEMSGSIZE:         msg = "WSAEMSGSIZE - Message too long"; break;
    case WSAEPROTOTYPE:       msg = "WSAEPROTOTYPE - Wrong protocol type for socket"; break;
    case WSAENOPROTOOPT:      msg = "WSAENOPROTOOPT - Bad protocol option"; break;
    case WSAEPROTONOSUPPORT:  msg = "WSAEPROTONOSUPPORT - Protocol not supported"; break;
    case WSAESOCKTNOSUPPORT:  msg = "WSAESOCKTNOSUPPORT - Socket type not supported"; break;
    case WSAEOPNOTSUPP:       msg = "WSAEOPNOTSUPP - Operation not supported"; break;
    case WSAEPFNOSUPPORT:     msg = "WSAEPFNOSUPPORT - Protocol family not supported"; break;
    case WSAEAFNOSUPPORT:     msg = "WSAEAFNOSUPPORT - Address family not supported"; break;
    case WSAEADDRINUSE:       msg = "WSAEADDRINUSE - Address already in use"; break;
    case WSAEADDRNOTAVAIL:    msg = "WSAEADDRNOTAVAIL - Cannot assign requested address"; break;
    case WSAENETDOWN:         msg = "WSAENETDOWN - Network is down"; break;
    case WSAENETUNREACH:      msg = "WSAENETUNREACH - Network is unreachable"; break;
    case WSAENETRESET:        msg = "WSAENETRESET - Network dropped connection"; break;
    case WSAECONNABORTED:     msg = "WSAECONNABORTED - Connection aborted"; break;
    case WSAECONNRESET:       msg = "WSAECONNRESET - Connection reset by peer"; break;
    case WSAENOBUFS:          msg = "WSAENOBUFS - No buffer space available"; break;
    case WSAEISCONN:          msg = "WSAEISCONN - Socket is already connected"; break;
    case WSAENOTCONN:         msg = "WSAENOTCONN - Socket is not connected"; break;
    case WSAESHUTDOWN:        msg = "WSAESHUTDOWN - Cannot send after socket shutdown"; break;
    case WSAETOOMANYREFS:     msg = "WSAETOOMANYREFS - Too many references"; break;
    case WSAETIMEDOUT:        msg = "WSAETIMEDOUT - Connection timed out"; break;
    case WSAECONNREFUSED:     msg = "WSAECONNREFUSED - Connection refused"; break;
    case WSAELOOP:            msg = "WSAELOOP - Too many symbolic links"; break;
    case WSAENAMETOOLONG:     msg = "WSAENAMETOOLONG - Name too long"; break;
    case WSAEHOSTDOWN:        msg = "WSAEHOSTDOWN - Host is down"; break;
    case WSAEHOSTUNREACH:     msg = "WSAEHOSTUNREACH - No route to host"; break;
    case WSAENOTEMPTY:        msg = "WSAENOTEMPTY - Directory not empty"; break;
    case WSAEPROCLIM:         msg = "WSAEPROCLIM - Too many processes"; break;
    case WSAEUSERS:           msg = "WSAEUSERS - Too many users"; break;
    case WSAEDQUOT:           msg = "WSAEDQUOT - Disk quota exceeded"; break;
    case WSAESTALE:           msg = "WSAESTALE - Stale file handle"; break;
    case WSAEREMOTE:          msg = "WSAEREMOTE - Object is remote"; break;
    case WSASYSNOTREADY:      msg = "WSASYSNOTREADY - Network subsystem is unavailable"; break;
    case WSAVERNOTSUPPORTED:  msg = "WSAVERNOTSUPPORTED - Winsock version not supported"; break;
    case WSANOTINITIALISED:   msg = "WSANOTINITIALISED - Winsock not initialized"; break;
    case WSAEDISCON:          msg = "WSAEDISCON - Graceful shutdown in progress"; break;
    case WSAENOMORE:          msg = "WSAENOMORE - No more results"; break;
    case WSAECANCELLED:       msg = "WSAECANCELLED - Operation cancelled"; break;
    case WSAEINVALIDPROCTABLE: msg = "WSAEINVALIDPROCTABLE - Invalid procedure table"; break;
    case WSAEINVALIDPROVIDER: msg = "WSAEINVALIDPROVIDER - Invalid service provider"; break;
    case WSAEPROVIDERFAILEDINIT: msg = "WSAEPROVIDERFAILEDINIT - Provider failed to initialize"; break;
    case WSASYSCALLFAILURE:   msg = "WSASYSCALLFAILURE - System call failure"; break;
    case WSASERVICE_NOT_FOUND: msg = "WSASERVICE_NOT_FOUND - Service not found"; break;
    case WSATYPE_NOT_FOUND:   msg = "WSATYPE_NOT_FOUND - Type not found"; break;
    case WSA_E_NO_MORE:       msg = "WSA_E_NO_MORE - No more results"; break;
    case WSA_E_CANCELLED:     msg = "WSA_E_CANCELLED - Operation cancelled"; break;
    case WSAEREFUSED:         msg = "WSAEREFUSED - Refused"; break;
    default:
        msg = "Unknown error code: " + to_string(code);
        break;
    }

    return msg;
}

string SetErrorMessageText(string errorMSg) {
    return errorMSg + GetErrorMessage();
}


bool GetServerByName(char* name, char* call, sockaddr* from, int* flen);


int main() {

    WSADATA wsaData;

    try {

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw SetErrorMessageText("Startup");
        }

        sockaddr_in server{};
        int flen = sizeof(server);
        bool found = GetServerByName((char*)"DESKTOP-SR5KB6L", (char*)"Hello", (sockaddr*)&server, &flen);

        if (found) {
            sockaddr_in* addr_in = (sockaddr_in*)&server;

            string ip = inet_ntoa(addr_in->sin_addr);
            int port = ntohs(addr_in->sin_port);

            cout << "Server IP: " + ip << endl;
            cout << "Server Port: " << port << endl;
        }

    }
    catch (string errorMessage) {
        cout << endl << errorMessage;
    }
}

bool GetServerByName(char* name, char* call, sockaddr* from, int* flen)
{
    SOCKET sock = INVALID_SOCKET;
    

    addrinfo hints{}, * res = nullptr;
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP

    int rc = getaddrinfo(name, DISCOVERY_PORT, &hints, &res);
    if (rc != 0 || !res) {
        return false;
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
        throw SetErrorMessageText("getServerByName, socket: ");

    //int timeout = 2000; // миллисекунды
    //setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));


    sockaddr_in localAddr{};
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(0);
    bind(sock, (sockaddr*)&localAddr, sizeof(localAddr));

    int sent = sendto(sock, call, (int)strlen(call), 0, res->ai_addr, (int)res->ai_addrlen);
    if (sent == SOCKET_ERROR) {
        closesocket(sock);
        freeaddrinfo(res);
        throw SetErrorMessageText("GetServerByName, socket: ");
    }


    char buffer[256];
    int bytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, from, flen);
    if (bytes == SOCKET_ERROR)
    {
        cout << SetErrorMessageText("Server not found");
        cout << endl;
        return false;
    }

    buffer[bytes] = '\0';

    bool ok = (strcmp(buffer, call) == 0);

    closesocket(sock);


    return ok;
}
