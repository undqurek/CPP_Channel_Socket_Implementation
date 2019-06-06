#pragma once


#include "basic/staticpipe.h"

#include <queue>


namespace core { namespace basic
{
    template<int const SEGMENT_LIMIT = 256> class DynamicPipe
    {
    private:
        uint32_t _size;
        std::queue<StaticPipe<SEGMENT_LIMIT>*> _queue;

    public:
        DynamicPipe();
        ~DynamicPipe();

        bool isEmpty();

        // Zwraca aktualna ilosc danych w buforze (sa to zapisane a nieodczytane jeszcze dane).
        //
        uint32_t getSize();

        // Odczytuje dane z bufora zmniejszajac jego rozmiar po przez usuniecie niepotrzebnych blokow.
        //
        // return : ilosc odczytanych danych
        uint32_t read(uint8_t *buffer, uint32_t length);

        // Zapisuje dane do bufora zwiekszajac jego rozmiar w razie potrzeby.
        //
        // return : ilosc zapisanych danych
        uint32_t write(uint8_t *buffer, uint32_t length);
    };


    template<int const SEGMENT_LIMIT> DynamicPipe<SEGMENT_LIMIT>::DynamicPipe()
    {
        this->_size = 0;
    }

    template<int const SEGMENT_LIMIT> DynamicPipe<SEGMENT_LIMIT>::~DynamicPipe()
    {
        this->_size = 0;

        while(true)
        {
            if(this->_queue.empty())
                return;

            StaticPipe<SEGMENT_LIMIT> *__pipe = this->_queue.front();

            this->_queue.pop();
            delete __pipe; // celowy zabieg pop
        }
    }

    template<int const SEGMENT_LIMIT> bool DynamicPipe<SEGMENT_LIMIT>::isEmpty()
    {
        return this->_size == 0;
    }

    template<int const SEGMENT_LIMIT> uint32_t DynamicPipe<SEGMENT_LIMIT>::getSize()
    {
        return this->_size;
    }

    template<int const SEGMENT_LIMIT> uint32_t DynamicPipe<SEGMENT_LIMIT>::read(uint8_t *buffer, uint32_t length)
    {
        uint32_t result = 0;

        while(this->_size > 0 && length > 0)
        {
            StaticPipe<SEGMENT_LIMIT> *__pipe = this->_queue.front();
            uint32_t tmp = __pipe->read(buffer, length);

            if(__pipe->isExhausted())
            {
                this->_queue.pop();
                delete __pipe; // celowy zabieg po pop()
            }

            buffer += tmp;
            length -= tmp;
            result += tmp;

            this->_size -= tmp;
        }

        return result;
    }

    template<int const SEGMENT_LIMIT> uint32_t DynamicPipe<SEGMENT_LIMIT>::write(uint8_t *buffer, uint32_t length)
    {
        if(length > 0)
        {
            StaticPipe<SEGMENT_LIMIT> *__pipe;

            if(this->_queue.empty())
                this->_queue.push(__pipe = new StaticPipe<SEGMENT_LIMIT>());

            else
            {
                __pipe = this->_queue.back();

                if(!__pipe->isWritable())
                    this->_queue.push(__pipe = new StaticPipe<SEGMENT_LIMIT>());
            }

            uint32_t result = 0;

            while(true)
            {
                uint32_t tmp = __pipe->write(buffer, length);

                buffer += tmp;
                result += tmp;
                length -= tmp;

                this->_size += tmp;

                if(length == 0)
                    break;

                this->_queue.push(__pipe = new StaticPipe<SEGMENT_LIMIT>());
            }

            return result;
        }

        return 0;
    }
}}
