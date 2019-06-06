#include "tcpserver.h"

#if SYSTEM == LINUX
#   include <unistd.h>
#   include <fcntl.h>
#endif


namespace core { namespace network { namespace nonblocking
{
    TcpServer::TcpServer()
    {
#if SYSTEM == LINUX
        this->_descriptor = -1;
#endif // LINUX

#if SYSTEM == WINDOWS
        ::WSADATA data = { 0 };
        ::WSAStartup(MAKEWORD(2, 2), &data); // inicjacja Winsock DLL dla procesu jesli nie zostala jeszcze zainicjowana

        this->_descriptor = INVALID_SOCKET;
#endif // WINDOWS

        this->_limit = 0;
        this->_port = 0;
    }

    TcpServer::~TcpServer()
    {
        this->stop();

#if SYSTEM == WINDOWS
        ::WSACleanup(); // sprzata po socket'ach
#endif // WINDOWS
    }

    bool TcpServer::isRun()
    {
#if SYSTEM == LINUX
        return this->_descriptor != -1;
#endif // LINUX

#if SYSTEM == WINDOWS
        return this->_descriptor != INVALID_SOCKET;
#endif // WINDOWS
    }

    uint32_t TcpServer::getLimit()
    {
        return this->_limit;
    }

    bool TcpServer::run(uint32_t limit, uint16_t port)
    {
        if(this->isRun())
            return false;

#if SYSTEM == LINUX
        // Otwiera socket przekazujac parametry:
        // address family: AF_INET <=> IP v4
        // type: SOCK_STREAM <=> strumien danych
        // protocol: 0 <=> TCP/IP
        this->_descriptor = ::socket(AF_INET, SOCK_STREAM, 0);

        // jesli nie udalo sie utworzyc socketu
        if(this->_descriptor == -1)
            return false;

        struct ::sockaddr_in address; // struktura reprezentujaca adres
        ::bzero(&address, sizeof(address));

        address.sin_family = AF_INET;// rodzina adresow IP v4
        address.sin_addr.s_addr = INADDR_ANY; // ustawiamy mozliwosc laczenia sie z dowolnego adresu
        address.sin_port = ::htons(port); // wykonujemy metode "host to network short" i ustawiamy port

        // binduje informacje dotyczace zasad laczenia sie klientow
        if (::bind(this->_descriptor, (::sockaddr *) &address, sizeof(address)) == -1)
        {
            ::close(this->_descriptor);
            this->_descriptor = -1;

            return false;
        }

        // uruchamia nasluchiwanie klientow
        if(::listen(this->_descriptor, limit) == -1)
        {
            ::close(this->_descriptor);
            this->_descriptor = -1;

            return false;
        }

        FD_ZERO(&this->_selector);
#endif // LINUX

#if SYSTEM == WINDOWS
        // Otwiera socket przekazujac parametry:
        // address family: AF_INET <=> IP v4
        // type: SOCK_STREAM <=> strumien danych
        // protocol: IPPROTO_TCP <=> TCP/IP
        this->_descriptor = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // jesli nie udalo sie utworzyc socketu
        if(this->_descriptor == INVALID_SOCKET)
            return false;

        struct ::sockaddr_in address; // struktura reprezentujaca adres
        ::ZeroMemory(&address, sizeof(struct ::sockaddr_in));

        address.sin_family = AF_INET;// rodzina adresow IP v4
        address.sin_addr.s_addr = INADDR_ANY; // ustawiamy mozliwosc laczenia sie z dowolnego adresu
        address.sin_port = ::htons(port); // wykonujemy metode "host to network short" i ustawiamy port

        // binduje informacje dotyczace zasad laczenia sie klientow
        if (::bind(this->_descriptor, (::sockaddr *) &address, sizeof(address)) == SOCKET_ERROR)
        {
            ::closesocket(this->_descriptor);
            this->_descriptor = INVALID_SOCKET;

            return false;
        }

        // uruchamia nasluchiwanie klientow
        if(::listen(this->_descriptor, limit) == SOCKET_ERROR)
        {
            ::closesocket(this->_descriptor);
            this->_descriptor = INVALID_SOCKET;

            return false;
        }

        FD_ZERO(&this->_selector);
#endif // WINDOWS

        this->_limit = limit;
        this->_port = port;

        return true;
    }

    bool TcpServer::stop()
    {
        if(this->isRun())
        {
#if SYSTEM == LINUX
            if(::shutdown(this->_descriptor, SHUT_RDWR) == 0
                && ::close(this->_descriptor) == 0)
            {
                this->_descriptor = -1;
                this->_limit = 0;

                return true;
            }
#endif // LINUX

#if SYSTEM == WINDOWS
            if(::shutdown(this->_descriptor, SD_BOTH) == 0
                && ::closesocket(this->_descriptor) == 0)
            {
                this->_descriptor = INVALID_SOCKET;
                this->_port = 0;

                return true;
            }
#endif // WINDOWS
        }

        return false;
    }

    bool TcpServer::probe(bool OUT &connection, long IN timeout)
    {
        bool result = false;

        if(this->isRun())
        {
#if SYSTEM == LINUX
            timeval time;

            time.tv_sec = timeout / 1000000;
            time.tv_usec = timeout - time.tv_sec * 1000000;

            FD_SET(this->_descriptor, &this->_selector);

            int tmp = ::select(this->_descriptor + 1, &this->_selector, NULL, NULL, &time);

            if(tmp == -1)
                result = false;

            else
            {
                if(tmp > 0)
                {
                    connection = FD_ISSET(this->_descriptor, &this->_selector);

                    return true;
                }

                result = true;
            }
#endif // LINUX

#if SYSTEM == WINDOWS
            timeval time;

            time.tv_sec = timeout / 1000000;
            time.tv_usec = timeout - time.tv_sec * 1000000;

            FD_SET(this->_descriptor, &this->_selector);

            int tmp = ::select(0, &this->_selector, NULL, NULL, &time);

            if(tmp == SOCKET_ERROR)
                result = false;

            else
            {
                if(tmp > 0)
                {
                    connection = FD_ISSET(this->_descriptor, &this->_selector);

                    return true;
                }

                result = true;
            }
#endif // WINDOWS
        }

        connection = false;

        return result;
    }

    uint16_t TcpServer::getPort()
    {
        return this->_port;
    }

    TcpSocket *TcpServer::accept()
    {
        if(this->isRun())
        {
#if SYSTEM == LINUX
            struct ::sockaddr_in address; // przechowuje adres podlaczonego klienta
            ::socklen_t addressSize = sizeof(::sockaddr_in); // rozmiar adresu

            ::bzero(&address, addressSize);

            // wyraza zgode na polaczenie oraz czeka na nowego klienta
            int descriptor = ::accept(this->_descriptor, (::sockaddr *)&address, &addressSize);

            // jesli laczenie klienta nie powiedlo sie
            if(descriptor == -1)
                return NULL;

            int mode = 1; // argument mowiacy o trybie nieblokujacym

//            // ustawia tryb nieblokujacy
//            if(::fcntl(descriptor, O_NONBLOCK, &mode) == -1)
//            {
//                ::close(this->_descriptor);

//                return NULL;
//            }

            char *ip = ::inet_ntoa(address.sin_addr);
#endif // LINUX

#if SYSTEM == WINDOWS
            struct ::sockaddr_in address; // przechowuje adres podlaczonego klienta
            int addressSize = sizeof(::sockaddr_in); // rozmiar adresu

            ::ZeroMemory(&address, addressSize);

            // wyraza zgode na polaczenie oraz czeka na nowego klienta
            ::SOCKET descriptor = ::accept(this->_descriptor, (::sockaddr *)&address, &addressSize);

            // jesli laczenie klienta nie powiedlo sie
            if(descriptor == INVALID_SOCKET)
                return NULL;

            u_long mode = 1; // argument mowiacy o trybie nieblokujacym

            // ustawia tryb nieblokujacy
            if(::ioctlsocket(this->_descriptor, FIONBIO, &mode) == SOCKET_ERROR)
            {
                ::closesocket(this->_descriptor);

                return false;
            }

            char *ip = ::inet_ntoa(address.sin_addr);
#endif // WINDOWS

            return new TcpSocket(descriptor, ip);
        }

        return NULL;
    }
}}}
