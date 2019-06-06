#pragma once


#include "basic/preprocesor.h"

#include <stdint.h>

#if SYSTEM == LINUX
#   include <stdlib.h>
#   include <stdio.h>
#   include <unistd.h>
#   include <string.h>
#   include <sys/socket.h>
#   include <sys/ioctl.h>
#   include <arpa/inet.h>
#   include <netdb.h>
//#   include "basic/helpers.h"
#endif

#if SYSTEM == WINDOWS
#   include <winsock2.h>
#   include <ws2tcpip.h>
#endif


namespace core { namespace network { namespace blocking
{
    class TcpClient;
    class TcpServer;

    class TcpSocket
    {
    private:

#if SYSTEM == LINUX
        int _descriptor;
#endif // LINUX

#if SYSTEM == WINDOWS
        ::SOCKET _descriptor;
#endif // WINDOWS

        char *_ip;

        friend class TcpClient;
        friend class TcpServer;

    private:

        TcpSocket();

#if SYSTEM == LINUX
        Socket(int desriptor, char *_ip);
#endif // LINUX

#if SYSTEM == WINDOWS
        TcpSocket(::SOCKET desriptor, char *_ip);
#endif // WINDOWS

	public:

        enum Type
        {
            T_IPv4,
            T_IPv6
        };

        enum Protocol
        {
            P_TCP,
            P_UDP
        };

    public:

        virtual ~TcpSocket();

        // Zwraca informacje czy socket jest otwarty.
        //
        bool isOpen();

        // Zwraca adres IP v4 z ktorym nawiazano polaczenie.
        //
        const char *__getIP();

        // Zamyka polaczenie.
        //
        virtual bool close();

        // Odczytuje odebrane dane.
        //
        bool read(const uint8_t *buffer, uint32_t length);

        // Wysyla dane.
        //
        bool write(const uint8_t *buffer, uint32_t length);

        // Zwraca informacje o ilosci dostepnych byte'ow do odczytania.
        //
        uint32_t getIncomingSize();
    };
}}}

