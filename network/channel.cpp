#include "channel.h"

#include <queue>


namespace core { namespace network
{
    using namespace nonblocking;

    Channel::Channel(int8_t index, ParallelSocket *__master)
    {
        this->_index = index;
        this->__master = __master;
    }

    uint32_t Channel::getInputSize()
    {
        return this->_input.getSize();
    }

    uint32_t Channel::getOutputSize()
    {
        return this->_output.getSize();
    }

    bool Channel::probeInput(uint32_t length)
    {
        uint32_t inputSize = this->_input.getSize();

        while(inputSize < length)
        {
            if(this->__master->fetch())
            {
                 uint32_t tmp = this->_input.getSize();

                 if(tmp > inputSize)
                 {
                     inputSize = tmp;

                     continue;
                 }
            }

            return false;
        }

        return true;
    }

    bool Channel::read(uint8_t *buffer, uint32_t length)
    {
        if(this->__master->isOpened())
        {
            while(length > 0)
            {
                Channel *__channel = this->__master->_channels[this->_index];

                if(__channel->_input.isEmpty())
                {
                    if(this->__master->fetch())
                        continue;

                    return false;
                }
                else
                {
                    uint32_t tmp = __channel->_input.read(buffer, length);

                    buffer += tmp;
                    length -= tmp;
                }
            }

            return true;
        }

        return false;
    }

    bool Channel::write(uint8_t *buffer, uint32_t length)
    {
        if(this->__master->isOpened())
        {
            Channel *__channel = this->__master->_channels[this->_index];

            if(__channel == NULL)
                return false;

            __channel->_output.write(buffer, length);

            return true;
        }

        return false;
    }
}}
