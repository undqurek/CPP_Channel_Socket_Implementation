#include <iostream>

#include "network/nonblocking/tcpserver.h"
#include "network/nonblocking/tcpclient.h"
//#include "network/parallelsocket.h"

using namespace std;
using namespace core::network;
using namespace core::network::nonblocking;

//enum ChannelType
//{
//    CT_Control = 0,
//    CT_Sensors,
//    CT_DualCamera,
//    CT_Camera360
//};

//// Reads available data.
////
//uint32_t read(Channel *channel, int8_t *buffer, uint32_t length)
//{
//    if(__channel0->fetch())
//    {
//        uint32_t tmp = __channel0->getInputSize();

//        if(__channel0->read(buffer, tmp))
//            return tmp;
//    }

//    return (-1);
//}

int main()
{
    TcpServer server;

    if(server.run(10, 5000)) // uruchamiamy serwer z maksymalna iloscia polaczen 10 na porcie 5000
    {
        cout << "----> Run server operation success." << endl;

        bool connection = false; // true oznacza oczekujace polaczenie

        while(true)
        {
            if(!server.probe(connection, 1000000)) // sprawdzamy czy pojawilo sie nowe polaczenie usypiajac watek na 1000000 mikrosekund (=1s) oczekujac polaczenia
                break;

            if(connection)
            {
                TcpSocket *client = server.accept(); // akceptujemy polaczenie

                if(client == NULL)
                    return 1;

                cout << "----> Socket accept operation success (client_address=" << client->__getIP() << ")." << endl;

                bool reading = false; // true oznacza oczekujace dane do odczytania
                bool writing = false; // true oznacza gotowosc do wysylania danych
                bool exception = false; // true oznacza sytuacje wyjatkowa

                uint8_t readBuffer[256]; // bufor wewnatrz ktorego umieszczane beda dane do odczytu
                uint8_t writeBuffer[256]; // bufor wewnatrz ktorego umieszczane beda dane do zapisu

                while(true)
                {
                    if(!client->probe(reading, writing, exception, 1000000)) // sprawdzamy czy pojawily sie nowe dane do odczytaqnia, gotowosc do wyslania danych lub sytuacja wyjatkowa usypiajac watek na 1000000 mikrosekund (=1s) oczekujac
                        break;

                    if(reading) // sprawdzamy czy nalezy przeprowadzic odczytywanie danych
                    {
                        uint32_t count = client->getInputDataSize(); // pobieramy ilosc danych do odczytania

                        cout << "[reading] bytes_to_read=" << count << endl;










                        if(count > 256)
                            count = 256;

                        uint32_t tmp = client->read(readBuffer, count); // odczytywanie odebraych bajtow; readBuffer to bufor z odczytanymi bajtami; count to ilosc danych do odczytania; tmp to ilosc odczytanych danych

                        if(tmp == 0) // sprawdzanie czy doszlo do zerwania polaczenia
                            break;

						{
                            cout << "BYTES: ";

                            for(int i = 0; i < tmp; ++i) // wypisywanie odczytanych bajtow
                                cout << (int)readBuffer[i] << " "; // wypisywana jest wartosc dziesietna bajtu

                            cout << endl;
                        }

                        {
                            cout << "ASCII: ";

                            for(int i = 0; i < tmp; ++i) // wypisywanie odczytanych bajtow
                                cout << (char)readBuffer[i]; // wypisywany jest znak ascii

                            cout << endl;
                        }










                    }

                    if(writing)
                    {
                        cout << "[writing] read to write" << endl;








                        uint32_t count = 0;

                        writeBuffer[count++] = 'A';
                        writeBuffer[count++] = 'l';
                        writeBuffer[count++] = 'a';
                        writeBuffer[count++] = ' ';
                        writeBuffer[count++] = 'm';
                        writeBuffer[count++] = 'a';
                        writeBuffer[count++] = ' ';
                        writeBuffer[count++] = 'k';
                        writeBuffer[count++] = 'o';
                        writeBuffer[count++] = 't';
                        writeBuffer[count++] = 'a';
						writeBuffer[count++] = '\n';
						writeBuffer[count++] = '\n';

                        uint32_t tmp = client->write(writeBuffer, count); // zapisywanie danych dowyslania; writeBuffer to bufor z wysylanymi bajtami; count to ilosc danych do wyslania; tmp to ilosc wyslanych danych

                        cout << "-> number_of_written_data=" << tmp << endl;

                        usleep(1000000); // spowolnienie wysylania danych











                    }

                    if(exception)
                    {
                        cout << "[exception]" << endl;
                    }
                }

                cout << "----> Socket close operation success (client_address=" << client->__getIP() << ")." << endl;

                delete client;
            }
        }

        server.stop();

        return 0;
    }
    else
        cout << "----> Run server operation error! Check port (close program with \"netstat -lap | grep 5000\" and \"kill -9 <PID>\") or wait few seconds." << endl;

/*
    std::cout << "I am server!!!" << std::endl;
    std::cout << std::endl;

    TcpServer server;

    if(server.run(26, 5000))
    {
        TcpSocket *client = server.accept();

        if(client == NULL)
            return 1;

        ParallelSocket socket(client);

        Channel *__channel0 = socket.__createChannel(CT_Control);
        Channel *__channel1 = socket.__createChannel(CT_Sensors);

        std::cout << std::endl;

        {
            / *

            if(__channel0->probeInput(2))
            {
                uint8_t header[2];

                __channel0->read(header, 2);

                uint8_t frameType = header[0];
                uint8_t frameSize = header[1];
            }

            or

            uint8_t buffer[256];
            uint32_t tmp = read(__channel0, buffer, 256);

            * /

            char message[256] = "Channel 0 message from server!!!";
            std::cout << "Channel 0: server->client message: " << message << std::endl;
            std::cout << "Channel 0: Write operation: " << (__channel0->write((uint8_t*)message, 256) ? "true" : "false") << std::endl;

            socket.flush(); // wymuszamy wyslanie wszytkich danych z buforow

            char buffer[256] = { 0 };
            std::cout << "Channel 0: Probe input operation: " << (__channel0->probeInput(256) ? "true" : "false") << std::endl;
            std::cout << "Channel 0: Read operation: " << (__channel0->read((uint8_t*)buffer, 256) ? "true" : "false") << std::endl;
            std::cout << "Channel 0: client->server message: " << buffer << std::endl;
        }

        {
            char message[256] = "Channel 1 message from server!!!";
            std::cout << "Channel 1: server->client message: " << message << std::endl;
            std::cout << "Channel 1: Write operation: " << (__channel1->write((uint8_t*)message, 256) ? "true" : "false") << std::endl;

            socket.flush(); // wymuszamy wyslanie wszytkich danych z buforow

            char buffer[256] = { 0 };
            std::cout << "Channel 1: Probe input operation: " << (__channel1->probeInput(256) ? "true" : "false") << std::endl;
            std::cout << "Channel 1: Read operation: " << (__channel1->read((uint8_t*)buffer, 256) ? "true" : "false") << std::endl;
            std::cout << "Channel 1: client->server message: " << buffer << std::endl;
        }

        std::cout << std::endl;

        socket.destroyChannel(CT_Control);
        socket.destroyChannel(CT_Sensors);

        delete client;

        server.stop();

        return 0;
    }
*/
    return 1;
}
