#pragma once


#include "nonblocking/tcpsocket.h"
#include "basic/action.h"


namespace core { namespace network
{
    class ChannelExtractor
    {
    private:
        uint32_t _state;

        uint8_t _index;
        uint8_t _offset;
        uint8_t _size;

        uint8_t _data[255];

    public:
        ChannelExtractor();

        bool isCompleted();

        uint8_t getIndex();
        uint8_t getSize();

        uint8_t *__getData();

        void reset();
        bool analyze(nonblocking::TcpSocket *socket);
    };
}}
