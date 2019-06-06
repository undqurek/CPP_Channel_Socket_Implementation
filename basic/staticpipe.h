#pragma once


#include <stdint.h>


namespace core { namespace basic
{
    template<int const LIMIT = 256> class StaticPipe
    {
    private:

        uint8_t _data[LIMIT];
        uint32_t _offset;
        uint32_t _size;

    public:

        StaticPipe();

        bool isReadable();
        bool isWritable();
        bool isExhausted();

        uint32_t getInputSize();
        uint32_t getOutputSize();

        uint8_t *__getInputData();
        uint8_t *__getOutputData();

        uint32_t read(uint8_t *buffer, uint32_t length);
        uint32_t write(uint8_t *buffer, uint32_t length);
    };



    template<int const LIMIT> StaticPipe<LIMIT>::StaticPipe()
    {
        this->_offset = 0;
        this->_size = 0;
    }

    template<int const LIMIT> bool StaticPipe<LIMIT>::isReadable()
    {
        return (this->_offset < this->_size);
    }

    template<int const LIMIT> bool StaticPipe<LIMIT>::isWritable()
    {
        return (this->_size < LIMIT);
    }

    template<int const LIMIT> bool StaticPipe<LIMIT>::isExhausted()
    {
        if(this->_size == LIMIT && this->_offset == LIMIT)
            return true;

        return false;
    }

    template<int const LIMIT> uint32_t StaticPipe<LIMIT>::getInputSize()
    {
        return LIMIT - this->_size;
    }

    template<int const LIMIT> uint32_t StaticPipe<LIMIT>::getOutputSize()
    {
        return this->_size - this->_offset;
    }

    template<int const LIMIT> uint8_t *StaticPipe<LIMIT>::__getInputData()
    {
        return this->_data + this->_size;
    }

    template<int const LIMIT> uint8_t *StaticPipe<LIMIT>::__getOutputData()
    {
        return this->_data + this->_offset;
    }

    template<int const LIMIT> uint32_t StaticPipe<LIMIT>::read(uint8_t *buffer, uint32_t length)
    {
        if(this->_offset < this->_size)
        {
            uint32_t space = this->_size - this->_offset;
            uint32_t limit = (length > space ? space : length);

            for(uint32_t i = 0; i < limit; ++i)
                buffer[i] = this->_data[this->_offset + i];

            this->_offset += limit;

            return limit;
        }

        return 0;
    }

    template<int const LIMIT> uint32_t StaticPipe<LIMIT>::write(uint8_t *buffer, uint32_t length)
    {
        if(this->_size < LIMIT)
        {
            uint32_t space = LIMIT - this->_size;
            uint32_t limit = (length > space ? space : length);

            for(uint32_t i = 0; i < limit; ++i)
                this->_data[this->_size + i] = buffer[i];

            this->_size += limit;

            return limit;
        }

        return 0;
    }
}}

