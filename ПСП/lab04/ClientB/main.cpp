#include<iostream>
#include<string>
#include <ws2tcpip.h>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

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


bool GetServer(char* name, short port, sockaddr* from, int* flen);


int main() {

    WSADATA wsaData;
    sockaddr serverAddr;
    int addrLen;
    try {

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw SetErrorMessageText("Startup: ");
        }

        bool hasServer = GetServer((char*)"Hello", 2000, &serverAddr, &addrLen);

        if (hasServer) {
            sockaddr_in* addr_in = (sockaddr_in*)&serverAddr;

            wchar_t ip[INET_ADDRSTRLEN];  // 16 байт для IPv4
            InetNtop(AF_INET, &(addr_in->sin_addr), ip, INET_ADDRSTRLEN);

            int port = ntohs(addr_in->sin_port);

            wcout << "Server IP: " << ip << endl;
            cout << "Server Port: " << port << endl;
        }

    }
    catch (string err) {
        cout << err;
    }

    WSACleanup();
}

bool GetServer(char* name, short port, sockaddr* from, int* flen)
{
    SOCKET clientSocket = INVALID_SOCKET;

    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET)
        throw SetErrorMessageText("socket ");


    BOOL broadcastEnable = TRUE;
    if (setsockopt(clientSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable)) == SOCKET_ERROR) {
        throw SetErrorMessageText("setsockopt(SO_BROADCAST): ");
    }

    sockaddr_in broadcastAddr;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(port);
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;


    int sent = sendto(clientSocket, name, strlen(name), 0,
        (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    if (sent == SOCKET_ERROR)
        throw SetErrorMessageText("sendto");

    char buffer[512];
    sockaddr_in fromAddr{};
    int fromLen = sizeof(fromAddr);

    int received = recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0,
        (sockaddr*)&fromAddr, &fromLen);
    if (received == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        if (err == WSAETIMEDOUT)
        {
            closesocket(clientSocket);
            return false;
        }
        throw SetErrorMessageText("recvfrom");
    }

    buffer[received] = '\0';

    if (strcmp(buffer, name) == 0)
    {
        memcpy(from, &fromAddr, sizeof(fromAddr));
        *flen = sizeof(fromAddr);
        closesocket(clientSocket);
        return true;
    }

    closesocket(clientSocket);
    return false;

}
