//#include "mutex.h"

//namespace core { namespace basic
//{
//    Mutex::Mutex()
//    {
//#if SYSTEM == WINDOWS
////        this->_mutex = ::CreateMutex(NULL, false, NULL);

//        ::pthread_mutex_init(&this->_mutex, NULL);
//#endif
//    }

//    Mutex::~Mutex()
//    {
//#if SYSTEM == WINDOWS
////        HANDLE tmp = this->_mutex;

////        if(tmp != NULL)
////        {
////            ::CloseHandle(tmp);
////            this->_mutex = NULL;
////        }

//        ::pthread_mutex_destroy(&this->_mutex);
//#endif
//    }

//    bool Mutex::lock()
//    {
//#if SYSTEM == WINDOWS
////        HANDLE tmp = this->_mutex;

////        if(tmp == NULL)
////            return false;

////        DWORD result = ::WaitForSingleObject(tmp, INFINITE);

////        if(result == WAIT_OBJECT_0)
////            return true;

//        if(::pthread_mutex_lock(&this->_mutex) == 0)
//            return true;
//#endif

//        return false;
//    }

//    bool Mutex::release()
//    {
//#if SYSTEM == WINDOWS
//        if(::pthread_mutex_unlock(&this->_mutex) == 0)
//            return true;
//#endif

//        return false;
//    }
//}}
