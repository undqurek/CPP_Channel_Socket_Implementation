#include "tcpclient.h"

#include <string>
#include <iostream>
#include <sstream>


namespace core { namespace network { namespace nonblocking
{
    TcpClient::TcpClient()
        : TcpSocket()
    {
#if SYSTEM == WINDOWS
        ::WSADATA wsaData = { 0 };
        ::WSAStartup(MAKEWORD(2, 2), &wsaData); // inicjacja Winsock DLL dla procesu jesli nie zostala jeszcze zainicjowana
#endif // WINDOWS

        this->_port = 0;
    }

    TcpClient::~TcpClient()
    {
        this->TcpSocket::~TcpSocket();

#if SYSTEM == WINDOWS
        ::WSACleanup(); // sprzata po socket'ach
#endif // WINDOWS
    }

    bool TcpClient::connect(uint32_t ipv4, uint16_t port)
    {
        if(this->isOpened())
            return false;

#if SYSTEM == LINUX
        // Otwieramy socket przekazujac parametry:
        // address family: AF_INET <=> IP v4
        // type: SOCK_STREAM <=> strumien danych
        // protocol: 0 <=> TCP/IP
        this->_descriptor = ::socket(AF_INET, SOCK_STREAM, 0);

        // jesli nie udalo sie utworzyc socketu
        if(this->_descriptor == -1)
            return false;

        //TODO: operacje nieblokujace (fcntl() i O_NONBLOCK)
//        if(this->_blocking == false)
//        {
//            u_long mode =  1; // argument mowiacy o trybie nieblokujacym

//            // ustawia tryb nieblokujacy
//            if(::ioctl(this->_descriptor, FIONBIO, &mode) == -1)
//            {
//                ::close(this->_descriptor);
//                this->_descriptor = -1;

//                return false;
//            }
//        }

        struct ::sockaddr_in address; // struktura reprezentujaca adres
        ::bzero(&address, sizeof(address));

        address.sin_family = AF_INET; // rodzina adresow IP v4
        address.sin_addr.s_addr = ipv4;
        address.sin_port = ::htons(port);

        // nawiazujemy polaczenie i sprawdzamy czy powiodla sie operacja
        if(::connect(this->_descriptor, (::sockaddr *)&address, sizeof(address)) == -1)
        {
            ::close(this->_descriptor);
            this->_descriptor = -1;

            return false;
        }

        //TODO: poprawic
        this->TcpSocket::_ip = ::inet_ntoa(*(struct in_addr *)&ipv4);
#endif // LINUX

#if SYSTEM == WINDOWS
        // Otwieramy socket przekazujac parametry:
        // address family: AF_INET <=> IP v4
        // type: SOCK_STREAM <=> strumien danych
        // protocol: IPPROTO_TCP <=> TCP/IP
        this->_descriptor = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // jesli nie udalo sie utworzyc socketu
        if(this->_descriptor == INVALID_SOCKET)
            return false;

        struct ::sockaddr_in address; // struktura reprezentujaca adres
        ::ZeroMemory(&address, sizeof(struct ::sockaddr_in));

        address.sin_family = AF_INET; // rodzina adresow IP v4
        address.sin_addr.s_addr = ipv4;
        address.sin_port = ::htons(port);

        // nawiazujemy polaczenie i sprawdzamy czy powiodla sie operacja
        if(::connect(this->_descriptor, (::sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
        {
            ::closesocket(this->_descriptor);
            this->_descriptor = INVALID_SOCKET;

            return false;
        }

        u_long mode = 1; // argument mowiacy o trybie nieblokujacym

        // ustawia tryb nieblokujacy
        if(::ioctlsocket(this->_descriptor, FIONBIO, &mode) == SOCKET_ERROR)
        {
            ::closesocket(this->_descriptor);
            this->_descriptor = INVALID_SOCKET;

            return false;
        }

        //TODO: poprawic
        this->TcpSocket::_ip = ::inet_ntoa(*(struct in_addr *)&ipv4);

        FD_ZERO(&this->_inletSelector);
        FD_ZERO(&this->_outletSelector);
        FD_ZERO(&this->_exceptionSelector);
#endif // WINDOWS

        this->_port = port;

        return true;
    }

    bool TcpClient::connect(const char *ipv4, uint16_t port)
    {
        uint32_t tmp = ::inet_addr(ipv4);

        return this->connect(tmp, port);
    }

    bool TcpClient::close()
    {
        if(this->TcpSocket::close())
        {
            this->_port = 0;
            return true;
        }

        return false;
    }
}}}
