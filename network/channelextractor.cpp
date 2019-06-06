#include "channelextractor.h"


namespace core { namespace network
{
    using namespace nonblocking;

    ChannelExtractor::ChannelExtractor()
    {
        this->reset();
    }

    bool ChannelExtractor::isCompleted()
    {
        return this->_offset == this->_size;
    }

    uint8_t ChannelExtractor::getIndex()
    {
        return this->_index;
    }

    uint8_t ChannelExtractor::getSize()
    {
        return this->_size;
    }

    uint8_t *ChannelExtractor::__getData()
    {
        return this->_data;
    }

    void ChannelExtractor::reset()
    {
        this->_state = 0;

        this->_index = 0;
        this->_offset = 0;
        this->_size = (-1);
    }

    bool ChannelExtractor::analyze(TcpSocket *socket)
    {
        uint32_t count = 0;

        while(true)
        {
            switch(this->_state)
            {
            case 0:
                count = socket->read(&this->_index, 1);

                if(count == TcpSocket::Error)
                    return false;

                if(count > 0)
                {
                    this->_state = 1;

                    break;
                }

                return true;

            case 1:
                count = socket->read(&this->_size, 1);

                if(count == TcpSocket::Error)
                    return false;

                if(count > 0)
                {
                    this->_state = 2;

                    break;
                }

                return true;

            case 2:
                if(this->_offset >= this->_size)
                     return true;

                count = socket->read(this->_data + this->_offset, this->_size - this->_offset);

                if(count == TcpSocket::Error)
                    return false;

                if(count > 0)
                {
                    this->_offset += count;

                    break;
                }

                return true;
            }
        }
    }
}}
