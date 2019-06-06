#include "parallelsocket.h"

#include "basic/blockingqueue.h"
#include "basic/staticpipe.h"

#include "nonblocking/tcpsocket.h"


namespace core { namespace network
{
    using namespace nonblocking;

    ParallelSocket::ParallelSocket(nonblocking::TcpSocket *__socket)
    {
        this->__socket = __socket;
        this->_channelsCount = 0;

        for(uint8_t i = 0; i < 10; ++i)
            this->_channels[i] = NULL;
    }

    ParallelSocket::~ParallelSocket()
    {
        for(uint8_t i = 0; i < 10; ++i)
        {
            delete this->_channels[i];
            this->_channels[i] = NULL;
        }

        this->_channelsCount = 0;
    }

    bool ParallelSocket::write(uint8_t *buffer, uint32_t length)
    {
        uint32_t offset = 0;

        while(length > 0)
        {
            bool writing = false;

            if(this->__socket->probeWriting(writing, 1000))
            {
                if(writing)
                {
                    uint32_t tmp = this->__socket->write(buffer + offset, length);

                    if(tmp == TcpSocket::Error || tmp == 0)
                        return false;

                    offset += tmp;
                    length -= tmp;
                }

                continue;
            }

            return false;
        }

        return true;
    }

    bool ParallelSocket::isOpened()
    {
        return this->__socket->isOpened();
    }

    Channel *ParallelSocket::__createChannel(uint8_t index)
    {
        if(index > 9)
            return NULL;

        if(this->_channels[index] == NULL)
            return (this->_channels[index] = new Channel(index, this));

        return NULL;
    }

    bool ParallelSocket::destroyChannel(uint8_t index)
    {
        if(index > 9 || this->_channels[index] == NULL)
            return false;

        delete this->_channels[index];
        this->_channels[index] = NULL;

        return true;
    }

    bool ParallelSocket::fetch()
    {
        bool reading = false;

        if(this->__socket->probeReading(reading, 1000))
        {
            if(reading == false)
                return true;

            if(this->_extractor.analyze(this->__socket))
            {
                if(this->_extractor.isCompleted())
                {
                    uint8_t index = this->_extractor.getIndex();

                    if(index < 10)
                    {
                        Channel *__channel = this->_channels[index];

                        if(__channel != NULL) // odczytujemy dane tylko dla utworzonych kanalow - reszte ignorujemy
                            __channel->_input.write(this->_extractor.__getData(), this->_extractor.getSize());
                    }

                    this->_extractor.reset();
                }

                return true;
            }
        }

        return false;
    }

    bool ParallelSocket::flush()
    {
        if(this->__socket->isOpened())
        {
            uint8_t buffer[255 + 2];

            while(true)
            {
                uint8_t counter = 0;

                for(uint8_t i = 0; i < 10; ++i)
                {
                    Channel *__channel = this->_channels[i];

                    if(__channel == NULL)
                        counter += 1;

                    else
                    {
                        uint32_t length = __channel->_output.read(buffer + 2, 255);

                        if(length > 0)
                        {
                            buffer[0] = i;
                            buffer[1] = length;

                            if(this->write(buffer, length + 2))
                                continue;

                            return false;
                        }
                        else
                            counter += 1;
                    }
                }

                if(counter == 10)
                    return true;
            }
        }

        return false;
    }
}}
