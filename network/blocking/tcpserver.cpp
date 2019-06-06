#include "tcpserver.h"


namespace core { namespace network { namespace blocking
{
    TcpServer::TcpServer()
    {
#if SYSTEM == LINUX
        this->_descriptor = -1;
#endif // LINUX

#if SYSTEM == WINDOWS
        ::WSADATA wsaData = { 0 };

        // inicjacja Winsock DLL dla procesu jesli nie zostala jeszcze zainicjowana
        ::WSAStartup(MAKEWORD(2, 2), &wsaData);

        this->_descriptor = INVALID_SOCKET;
#endif // WINDOWS

        this->_limit = 0;
        this->_port = 0;
    }

    TcpServer::~TcpServer()
    {
        this->stop();

#if SYSTEM == WINDOWS
        // sprzata po socket'ach
        ::WSACleanup();
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

    bool TcpServer::isRun()
    {
#if SYSTEM == LINUX
        return this->_descriptor != -1;
#endif // LINUX

#if SYSTEM == WINDOWS
        return this->_descriptor != INVALID_SOCKET;
#endif // WINDOWS
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

            // wyraza zgode na polaczenie, oraz czeka na nowego klienta
            int descriptor = ::accept(this->_descriptor, (::sockaddr *)&address, &addressSize);

            //TODO: operacje nieblokujace (fcntl() i O_NONBLOCK)

            // jesli laczenie klienta nie powiedlo sie
            if(descriptor == -1)
                return NULL;

            char *ip = ::inet_ntoa(address.sin_addr);
#endif // LINUX

#if SYSTEM == WINDOWS
            struct ::sockaddr_in address; // przechowuje adres podlaczonego klienta
            int addressSize = sizeof(::sockaddr_in); // rozmiar adresu

            ::ZeroMemory(&address, addressSize);

            // wyraza zgode na polaczenie, oraz czeka na nowego klienta
            ::SOCKET descriptor = ::accept(this->_descriptor, (::sockaddr *)&address, &addressSize);

            //TODO: operacje nieblokujace (fcntl() i O_NONBLOCK)

            // jesli laczenie klienta nie powiedlo sie
            if(descriptor == INVALID_SOCKET)
                return NULL;

            char *ip = ::inet_ntoa(address.sin_addr);
#endif // WINDOWS

            return new TcpSocket(descriptor, ip);
        }

        return NULL;
    }
}}}
