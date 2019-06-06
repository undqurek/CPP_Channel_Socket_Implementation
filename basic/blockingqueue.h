//#pragma once


//#include "basic/mutex.h"
//#include "basic/semaphore.h"

//#include <queue>


//namespace core { namespace basic
//{
//    // Jednokierunkowa kolejka.
//    //
//    template<class T> class BlockingQueue
//    {
//    private:

//        Mutex _mutex;
//        Semaphore _semaphore;

//        std::queue<T> _queue;

//    public:

//        bool isEmpty();

//        // Zwraca rozmiar kolejki.
//        //
//        uint32_t getSize();

//        // Pobiera element z kolejki bez usuwania.
//        //
//        T getHead();

//        // Pobiera element z kolejki bez usuwania.
//        //
//        T getTail();

//        // Wstawia element do kolejki.
//        //
//        void enqueue(T value);

//        // Usuwa element z kolejki.
//        //
//        T dequeue();

//        // Usuwa elemen z kolejki.
//        //
//        void remove();
//    };

//    template<class T> bool BlockingQueue<T>::isEmpty()
//    {
//        bool result = true;

//        if(this->_mutex.lock())
//        {
//            result = this->_queue.empty();

//            this->_mutex.release();
//        }

//        return result;
//    }

//    template<class T> uint32_t BlockingQueue<T>::getSize()
//    {
//        uint32_t result = 0;

//        if(this->_mutex.lock())
//        {
//            result = this->_queue.size();

//            this->_mutex.release();
//        }

//        return result;
//    }

//    template<class T> T BlockingQueue<T>::getHead()
//    {
//        T result;

//        //TODO: zredkowac liczbe mutex'ow
//        if(this->_mutex.lock())
//        {
//            if(this->_queue.empty())
//            {
//                this->_mutex.release();
//                this->_semaphore.wait();
//            }
//            else
//                this->_mutex.release();

//            if(this->_mutex.lock())
//            {
//                result = this->_queue.front();

//                this->_mutex.release();
//            }
//        }

//        return result;
//    }

//    template<class T> T BlockingQueue<T>::getTail()
//    {
//        T result;

//        //TODO: zredkowac liczbe mutex'ow
//        if(this->_mutex.lock())
//        {
//            if(this->_queue.empty())
//            {
//                this->_mutex.release();
//                this->_semaphore.wait();
//            }
//            else
//                this->_mutex.release();

//            if(this->_mutex.lock())
//            {
//                result = this->_queue.back();

//                this->_mutex.release();
//            }
//        }

//        return result;
//    }

//    template<class T> void BlockingQueue<T>::enqueue(T value)
//    {
//        //TODO: zredkowac liczbe mutex'ow
//        if(this->_mutex.lock())
//        {
//            this->_queue.push(value);
//            this->_mutex.release();

//            this->_semaphore.signal();
//        }
//    }

//    template<class T> T BlockingQueue<T>::dequeue()
//    {
//        T result;

//        //TODO: zredkowac liczbe mutex'ow
//        if(this->_mutex.lock())
//        {
//            if(this->_queue.empty())
//            {
//                this->_mutex.release();
//                this->_semaphore.wait();
//            }
//            else
//                this->_mutex.release();

//            if(this->_mutex.lock())
//            {
//                result = this->_queue.front();

//                this->_queue.pop();
//                this->_mutex.release();
//            }
//        }

//        return result;
//    }

//    template<class T> void BlockingQueue<T>::remove()
//    {
//        if(this->_mutex.lock())
//        {
//            if(this->_queue.empty())
//                this->_mutex.release();

//            else
//            {
//                this->_queue.pop();
//                this->_mutex.release();
//            }
//        }
//    }
//}}
