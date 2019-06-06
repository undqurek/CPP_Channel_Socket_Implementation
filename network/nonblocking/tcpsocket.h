#ifndef HEAD_1527097892
#define HEAD_1527097892

#include <stdint.h>

#include "basic/preprocesor.h"


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


namespace core { namespace network { namespace nonblocking
{
    class TcpClient;
    class TcpServer;

    class TcpSocket
    {
    private:

#if SYSTEM == LINUX
        int _descriptor;

        ::fd_set _readingSelector; // pomaga w wykrywaniu gotowosci do odczytu
        ::fd_set _writingSelector; // pomaga w wykrywaniu gotowosci do wyslania
        ::fd_set _exceptionSelector; // pomaga w wykrywaniu nietypowych sytuacji (nie mylic z throw Exception)
#endif // LINUX

#if SYSTEM == WINDOWS
        ::SOCKET _descriptor;

        ::fd_set _readingSelector; // pomaga w wykrywaniu gotowosci do odczytu
        ::fd_set _writingSelector; // pomaga w wykrywaniu gotowosci do wyslania
        ::fd_set _exceptionSelector; // pomaga w wykrywaniu nietypowych sytuacji (nie mylic z throw Exception)
#endif // WINDOWS

        char *_ip;

        friend class TcpClient;
        friend class TcpServer;

    private:

        TcpSocket();

#if SYSTEM == LINUX
        TcpSocket(int desriptor, char *_ip);
#endif // LINUX

#if SYSTEM == WINDOWS
        TcpSocket(::SOCKET desriptor, char *_ip);
#endif // WINDOWS

    public:

        static const uint32_t Error = (-1);

        enum Type
        {
            T_IPv4,
            T_IPv6
        };

    public:

        virtual ~TcpSocket();

        // Zwraca informacje czy socket jest otwarty.
        //
        bool isOpened();

        // Zwraca adres IP v4 z ktorym nawiazano polaczenie.
        //
        const char *__getIP();

        // Zamyka polaczenie.
        //
        virtual bool close();

        // Sprawdza stan gotowosci do odczytu.
        //
        // reading - zwraca informacje o gotowosci do odczytu
        // timeout - czas uspienia funkcji w mikrosekundach
        bool probeReading(bool OUT &reading, long IN timeout = 0);

        // Sprawdza stan gotowosci do zapisu.
        //
        // writing - zwraca informacje o gotowosci do zapisu
        // timeout - czas uspienia funkcji w mikrosekundach
        bool probeWriting(bool OUT &writing, long IN timeout = 0);

        // Sprawdza stany gotowosci do zapisu i odczytu.
        //
        // reading - zwraca informacje o gotowosci do odczytu
        // writing - zwraca informacje o gotowosci do zapisu
        // exception - zwraca informacje zaistnialych sytuacjach wyjatkowych (nie mylic z throw Exception)
        // timeout - czas uspienia funkcji w mikrosekundach
        bool probe(bool OUT &reading, bool OUT &writing, bool OUT &exception, long IN timeout = 0);

        // Odczytuje odebrane dane.
        //
        // return: number of read bytes or 0xFFFFFFFF for error (0 oznacza zerwanie polaczenia jesli uzyto funkcji probe - nie ustawiac length na 0!)
        uint32_t read(const uint8_t *buffer, uint32_t length);

        // Wysyla dane.
        //
        // return: number of written bytes or 0xFFFFFFFF for connection error
        uint32_t write(const uint8_t *buffer, uint32_t length);

        // Zwraca informacje o ilosci dostepnych byte'ow do odczytania.
        //
        uint32_t getInputDataSize();
    };
}}}

#endif HEAD_1527097892
