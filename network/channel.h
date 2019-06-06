#pragma once


#include "parallelsocket.h"
#include "channelextractor.h"
#include "basic/dynamicpipe.h"

#include <stdint.h>


namespace core { namespace network
{
    class ParallelSocket;

    class Channel
    {
    private:

        int8_t _index;
        ParallelSocket *__master;

        basic::DynamicPipe<255> _input; // bufor wejsciowy kanalu
        basic::DynamicPipe<255> _output; // bufor wyjsciowy kanalu

        friend class ParallelSocket;

    private:

        Channel(int8_t index, ParallelSocket *__master);

    public:

        // Zwraca ilosc danych znajdujacych sie obecnie wewnatrz buforu odczytu.
        //
        uint32_t getInputSize();

        // Zwraca ilosc danych znajdujacych sie obecnie wewnatrz buforu zapisu.
        //
        uint32_t getOutputSize();

        // Sprawdza dostepnosc danych w buforze wejsciowym kanalu rodzielajac na kanaly czekajace dane w buforze socket'u.
        //
        bool probeInput(uint32_t length);

        // Odczytuje dane (jesli bufory wejsciowe kanalu sa puste to czekaja na dotarcie danych).
        //
        bool read(uint8_t *buffer, uint32_t length);

        // Zapisuje dane do bufora wyjsciowego kanalu. Aby dane te zostaly wyslane do odbiorcy nalezy wykonac
        // flush na obiekcie nadrzednym (typu ParallelSocket). Aby zoptymalizowac wysylanie nalezy wykonac flush
        // dopiero po zapisie wszystkich danych we wszystkich kanalach.
        //
        bool write(uint8_t *buffer, uint32_t length);
    };
}}
