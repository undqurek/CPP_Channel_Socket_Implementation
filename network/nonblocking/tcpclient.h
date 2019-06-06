#ifndef HEAD_1527097359
#define HEAD_1527097359

#include <stdint.h>

#include "tcpsocket.h"


namespace core { namespace network { namespace nonblocking
{
    class TcpClient
        : public TcpSocket
    {
    private:

        uint16_t _port;

    public:

        TcpClient();
        virtual ~TcpClient();

        // Nawiazuje polaczenie ze wskazanym adresem.
        //
        bool connect(uint32_t ipv4, uint16_t port);

        // Nawiazuje polaczenie ze wskazanym adresem.
        //
        bool connect(const char *ipv4, uint16_t port);

        // Zamyka polaczenie.
        //
        bool close();
    };
}}}

#endif HEAD_1527097359
