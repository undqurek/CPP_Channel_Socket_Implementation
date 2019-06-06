#include <iostream>

#include "network/nonblocking/tcpserver.h"
#include "network/nonblocking/tcpclient.h"
#include "network/parallelsocket.h"

using namespace std;
using namespace core::network;
using namespace core::network::nonblocking;

enum ChannelType
{
    CT_Control = 0,
    CT_Sensors,
    CT_DualCamera,
    CT_Camera360
};

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
    std::cout << "I am client!!!" << std::endl;
    std::cout << std::endl;

    TcpClient client;

    if(client.connect("127.0.0.1", 5000))
    {
        ParallelSocket socket(&client);

        Channel *__channel0 = socket.__createChannel(CT_Control);
        Channel *__channel1 = socket.__createChannel(CT_Sensors);

        {
            /*

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

            */

            char buffer[256] = { 0 };
            std::cout << "Channel 0: Probe input operation: " << (__channel0->probeInput(256) ? "true" : "false") << std::endl;
            std::cout << "Channel 0: Read operation: " << (__channel0->read((uint8_t*)buffer, 256) ? "true" : "false") << std::endl;
            std::cout << "Channel 0: server->client message: " << buffer << std::endl;

            char message[256] = "Channel 0 message from client!!!";
            std::cout << "Channel 0: client->server message: " << message << std::endl;
            std::cout << "Channel 0: Write operation: " << (__channel0->write((uint8_t*)message, 256) ? "true" : "false") << std::endl;

            socket.flush(); // wymuszamy wyslanie wszytkich danych z buforow
        }

        std::cout << std::endl;

        {
            char buffer[256] = { 0 };
            std::cout << "Channel 1: Probe input operation: " << (__channel1->probeInput(256) ? "true" : "false") << std::endl;
            std::cout << "Channel 1: Read operation: " << (__channel1->read((uint8_t*)buffer, 256) ? "true" : "false") << std::endl;
            std::cout << "Channel 1: server->client message: " << buffer << std::endl;

            char message[256] = "Channel 1 message from client!!!";
            std::cout << "Channel 1: client->server message: " << message << std::endl;
            std::cout << "Channel 1: Write operation: " << (__channel1->write((uint8_t*)message, 256) ? "true" : "false") << std::endl;

            socket.flush(); // wymuszamy wyslanie wszytkich danych z buforow
        }

        std::cout << std::endl;

        socket.destroyChannel(CT_Control);
        socket.destroyChannel(CT_Sensors);

        client.close();

        return 0;
    }

    return 1;
}
