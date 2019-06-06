//#include "semaphore.h"

//#include <iostream>


//namespace core { namespace basic
//{
//    Semaphore::Semaphore()
//    {
//        this->_mutex = new Mutex();
//        this->__mutex = this->_mutex;

//        ::pthread_cond_init(&this->_cond, NULL);
//    }

//    Semaphore::Semaphore(Mutex *__mutex)
//    {
//        this->_mutex = NULL;
//        this->__mutex = __mutex;

//        ::pthread_cond_init(&this->_cond, NULL);
//    }

//    Semaphore::~Semaphore()
//    {
//        this->__mutex = NULL;

//        if(this->_mutex != NULL)
//        {
//            delete this->_mutex;
//            this->_mutex = NULL;
//        }

//        ::pthread_cond_destroy(&this->_cond);
//    }

//    bool Semaphore::wait()
//    {
//        if(this->__mutex == NULL)
//            return false;

//        if(this->__mutex->lock())
//        {
//            ::pthread_cond_wait(&this->_cond, &this->__mutex->_mutex);

//            return this->__mutex->release();
//        }

//        return false;
//    }

//    bool Semaphore::signal()
//    {
//        if(this->__mutex == NULL)
//            return false;

//        if(this->__mutex->lock())
//        {
//            ::pthread_cond_signal(&this->_cond);

//            return this->__mutex->release();
//        }

//        return false;
//    }
//}}
