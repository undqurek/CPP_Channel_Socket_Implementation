#ifndef HEAD_1527097874
#define HEAD_1527097874

#include <stdint.h>

#include "basic/preprocesor.h"
#include "tcpsocket.h"


namespace core { namespace network { namespace nonblocking
{
    class TcpServer
    {
    private:

#if SYSTEM == LINUX
        int _descriptor;

        ::fd_set _selector; // pomaga w monitorowaniu przychodzacych polaczen
#endif // LINUX

#if SYSTEM == WINDOWS
        ::SOCKET _descriptor; // identyfikator soketu

        ::fd_set _selector; // pomaga w monitorowaniu przychodzacych polaczen
#endif // WINDOWS

        uint32_t _limit; // limit polaczen
        uint16_t _port;

    public:

        TcpServer();
        virtual ~TcpServer();

        // Sprawdza czy serwer jest uruchomiony.
        //
        bool isRun();

        // Zwraca limit polaczen do uruchomionego serwera.
        //
        uint32_t getLimit();

        // Zwraca numer portu na ktorym uruchomiono serwer.
        //
        uint16_t getPort();

        // Uruchamia serwer pod wskazanym portem.
        //
        bool run(uint32_t limit, uint16_t port);

        // Zatrzymuje serwer. Wymagane jest aby najpierw zakonczyc wszystkie polaczenia w klientami.
        //
        bool stop();

        // Sprawdza oczekujace polczenia.
        //
        // connection - zwraca informacje o czekajacych polaczeniach
        // timeout - czas uspienia funkcji w mikrosekundach
        bool probe(bool OUT &connection, long IN timeout = 0);

        // Czeka na polaczenie sie klienta i zwraca socket za pomoca, ktorego mozna komunikawac sie z klientem lub NULL w przypadku bledu.
        //
        TcpSocket *accept();
    };
}}}

#endif HEAD_1527097874
