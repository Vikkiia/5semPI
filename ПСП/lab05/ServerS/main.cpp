#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<string>
#include <ws2tcpip.h>
#include<vector>
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


struct ServerInfo {
    wstring ip;
    int port;
};

bool checkOtherServers(const char* name, short port, std::vector<ServerInfo>& servers);
bool GetRequestFromClient(SOCKET s, const char* expectedMsg, sockaddr* from, int* flen);
bool PutAnswerToClient(SOCKET s, const char* msg, sockaddr* to, int* tlen);

int main() {

    WSAData wsadata;
    string serverName = "Hello";

    SOCKET serverSocket;
    SOCKADDR_IN add_serv;
    sockaddr client;
    int len = sizeof(client);

    try {
        if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
            throw SetErrorMessageText("Startup: ");
        }

        //std::vector<ServerInfo> servers;
/*        if (checkOtherServers(serverName.c_str(), 2000, servers)) {

            cout << "server count: " << servers.size() << endl;
            for (auto server : servers)
            {
                wcout << server.ip << ":" << server.port << endl;
            *///}
        //}
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
            cout << SetErrorMessageText("getHostName: ");
            cout << endl;
        }
        else {
            cout << "Server host name: " << hostname << endl;
        }

        serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (serverSocket == INVALID_SOCKET)
            throw SetErrorMessageText("socket");

        add_serv.sin_family = AF_INET;
        add_serv.sin_port = htons(2000);
        add_serv.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (LPSOCKADDR)&add_serv, sizeof(add_serv)) == SOCKET_ERROR)
            throw SetErrorMessageText("bind");

        while (true) {

            bool clientRequest = GetRequestFromClient(serverSocket, serverName.c_str(), &client, &len);
            if (clientRequest) {

                sockaddr_in* addr_in = (sockaddr_in*)&client;
                char* ip_bytes = (char*)&(addr_in->sin_addr);

                hostent* host = gethostbyaddr(ip_bytes, sizeof(addr_in->sin_addr), AF_INET);
                if (!host) {
                    cout << SetErrorMessageText("gethostbyaddr: ");
                    cout << endl;
                }
                else {
                    cout << "client host name: " << host->h_name << endl;
                }

                wchar_t ip[INET_ADDRSTRLEN];  
                InetNtop(AF_INET, &(addr_in->sin_addr), ip, INET_ADDRSTRLEN);

                int port = ntohs(addr_in->sin_port);

                wcout << "Client IP: " << ip << endl;
                cout << "Client Port: " << port << endl;


                PutAnswerToClient(serverSocket, serverName.c_str(), &client, &len);
            }
        }
    }
    catch (string errMessage) {
        cout << errMessage << endl;

        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
    }

    WSACleanup();

}

bool checkOtherServers(const char* name, short port, std::vector<ServerInfo>& servers)
{
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) return false;

    BOOL broadcastEnable = TRUE;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable));

    sockaddr_in broadcastAddr;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(port);
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;

    int sent = sendto(sock, name, (int)strlen(name), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    if (sent == SOCKET_ERROR) { std::cout << SetErrorMessageText("sendto: "); closesocket(sock); return false; }

    DWORD timeout = 5000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    char buffer[256];
    sockaddr_in fromAddr;
    int fromLen = sizeof(fromAddr);

    while (true) {
        int ret = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&fromAddr, &fromLen);
        if (ret == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAETIMEDOUT) break;
            else {
                closesocket(sock);
                return false;
            }
        }

        buffer[ret] = '\0';


        if (strcmp(buffer, name) == 0) {
            wchar_t ip[INET_ADDRSTRLEN];
            InetNtop(AF_INET, &(fromAddr.sin_addr), ip, INET_ADDRSTRLEN);

            int port = ntohs(fromAddr.sin_port);

            servers.push_back({ ip, port });
        }
    }


    closesocket(sock);
    return true;
}

bool GetRequestFromClient(SOCKET s, const char* expectedMsg, sockaddr* from, int* flen)
{
    char buffer[512];
    int ret;

    *flen = sizeof(*from);

    ret = recvfrom(s, buffer, sizeof(buffer) - 1, 0, from, flen);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAETIMEDOUT)
            return false;
        throw SetErrorMessageText("recvfrom");
    }

    buffer[ret] = '\0';
    printf("Received: %s\n", buffer);

    return strcmp(buffer, expectedMsg) == 0;
}


bool PutAnswerToClient(SOCKET s, const char* msg, sockaddr* to, int* tlen)
{
    int sent = sendto(s, msg, strlen(msg), 0, to, *tlen);
    if (sent == SOCKET_ERROR) {
        cout << SetErrorMessageText("sendto: ");
        return false;
    }

    return true;
}
