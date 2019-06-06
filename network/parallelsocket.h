#pragma once


#include "basic/preprocesor.h"
#include "nonblocking/tcpsocket.h"
#include "channelextractor.h"
#include "channel.h"

#include <stdint.h>


namespace core { namespace network
{
    class Channel;

    class ParallelSocket
    {
    private:

        nonblocking::TcpSocket *__socket;

        ChannelExtractor _extractor;

        uint8_t _channelsCount;
        Channel *_channels[10];

        friend class Channel;

    public:

        ParallelSocket(nonblocking::TcpSocket *__socket);
        ~ParallelSocket();

    private:

        // Wysyla/zapisuje dane do socketu czekajac az operacja sie zakonczy.
        //
        bool write(uint8_t *buffer, uint32_t length);

    public:

        // Zwraca informacje czy socket jest otwarty.
        //
        bool isOpened();

        // Tworzy kanal komunikacji.
        //
        Channel *__createChannel(uint8_t index);

        // Niszczy kanal komunikacji.
        //
        bool destroyChannel(uint8_t index);

        // Wymusza odczytanie danych dla kanalow.
        //
        bool fetch();

        // Wymusza wyslanie danych lezacych w buforach.
        //
        bool flush();
    };
}}
