#include "tcpsocket.h"

namespace core { namespace network { namespace nonblocking
{
    TcpSocket::TcpSocket()
    {
#if SYSTEM == LINUX
        this->_descriptor = -1;
#endif // LINUX

#if SYSTEM == WINDOWS
        this->_descriptor = INVALID_SOCKET;
#endif // WINDOWS

        this->_ip = NULL;
    }

#if SYSTEM == LINUX
    TcpSocket::TcpSocket(int descriptor, char *_ip)
    {
        this->_descriptor = descriptor;
        this->_ip = _ip;

        FD_ZERO(&this->_readingSelector);
        FD_ZERO(&this->_writingSelector);
        FD_ZERO(&this->_exceptionSelector);
    }
#endif // LINUX

#if SYSTEM == WINDOWS
    TcpSocket::TcpSocket(::SOCKET descriptor, char *_ip)
    {
        this->_descriptor = descriptor;
        this->_ip = _ip;

        FD_ZERO(&this->_readingSelector);
        FD_ZERO(&this->_writingSelector);
        FD_ZERO(&this->_exceptionSelector);
    }
#endif // WINDOWS

    TcpSocket::~TcpSocket()
    {
        this->close();
    }

    const char *TcpSocket::__getIP()
    {
        return this->_ip;
    }

    bool TcpSocket::close()
    {
        if(this->isOpened())
        {
            //TODO: poprawne zwalnainie pamieci
    //            Cleaner::deleteArray(this->_ip);

#if SYSTEM == LINUX
            if(::shutdown(this->_descriptor, SHUT_RDWR) == 0
                && ::close(this->_descriptor) == 0)
            {
                this->_descriptor = -1;

                return true;
            }
#endif // LINUX

#if SYSTEM == WINDOWS
            if(::shutdown(this->_descriptor, SD_BOTH) == 0
                && ::closesocket(this->_descriptor) == 0)
            {
                this->_descriptor = INVALID_SOCKET;

                return true;
            }
#endif // WINDOWS
        }

        return false;
    }

    bool TcpSocket::probeReading(bool OUT &reading, long timeout)
    {
        bool result = false;

        if(this->isOpened())
        {
#if SYSTEM == LINUX
            timeval time;

            time.tv_sec = timeout / 1000000;
            time.tv_usec = timeout - time.tv_sec * 1000000;

            FD_SET(this->_descriptor, &this->_readingSelector);

            int tmp = ::select(0, &this->_readingSelector, NULL, NULL, &time);

            if(tmp == -1)
                result = false;

            else
            {
                if(tmp > 0)
                {
                    reading = FD_ISSET(this->_descriptor, &this->_readingSelector);

                    return true;
                }

                result = true;
            }
#endif // LINUX

#if SYSTEM == WINDOWS
            timeval time;

            time.tv_sec = timeout / 1000000;
            time.tv_usec = timeout - time.tv_sec * 1000000;

            FD_SET(this->_descriptor, &this->_readingSelector);

            int tmp = ::select(0, &this->_readingSelector, NULL, NULL, &time);

            if(tmp == SOCKET_ERROR)
                result = false;

            else
            {
                if(tmp > 0)
                {
                    reading = FD_ISSET(this->_descriptor, &this->_readingSelector);

                    return true;
                }

                result = true;
            }
#endif // WINDOWS
        }

        reading = false;

        return result;
    }

    bool TcpSocket::probeWriting(bool OUT &writing, long timeout)
    {
        bool result = false;

        if(this->isOpened())
        {
#if SYSTEM == LINUX
            timeval time;

            time.tv_sec = timeout / 1000000;
            time.tv_usec = timeout - time.tv_sec * 1000000;

            FD_SET(this->_descriptor, &this->_writingSelector);

            int tmp = ::select(0, NULL, &this->_writingSelector, NULL, &time);

            if(tmp == -1)
                result = false;

            else
            {
                if(tmp > 0)
                {
                    writing = FD_ISSET(this->_descriptor, &this->_writingSelector);

                    return true;
                }

                result = true;
            }
#endif // LINUX

#if SYSTEM == WINDOWS
            timeval time;

            time.tv_sec = timeout / 1000000;
            time.tv_usec = timeout - time.tv_sec * 1000000;

            FD_SET(this->_descriptor, &this->_writingSelector);

            int tmp = ::select(0, NULL, &this->_writingSelector, NULL, &time);

            if(tmp == SOCKET_ERROR)
                result = false;

            else
            {
                if(tmp > 0)
                {
                    writing = FD_ISSET(this->_descriptor, &this->_writingSelector);

                    return true;
                }

                result = true;
            }
#endif // WINDOWS
        }

        writing = false;

        return result;
    }

    bool TcpSocket::probe(bool OUT &reading, bool OUT &writing, bool OUT &exception, long IN timeout)
    {
        bool result = false;

        if(this->isOpened())
        {
#if SYSTEM == LINUX
            timeval time;

            time.tv_sec = timeout / 1000000;
            time.tv_usec = timeout - time.tv_sec * 1000000;

            FD_SET(this->_descriptor, &this->_readingSelector);
            FD_SET(this->_descriptor, &this->_writingSelector);
            FD_SET(this->_descriptor, &this->_exceptionSelector);

            int tmp = ::select(this->_descriptor + 1, &this->_readingSelector, &this->_writingSelector, &this->_exceptionSelector, &time);

            if(tmp == -1)
                result = false;

            else
            {
                if(tmp > 0)
                {
                    reading = FD_ISSET(this->_descriptor, &this->_readingSelector);
                    writing = FD_ISSET(this->_descriptor, &this->_writingSelector);
                    exception = FD_ISSET(this->_descriptor, &this->_exceptionSelector);

                    return true;
                }

                result = true;
            }
#endif // LINUX

#if SYSTEM == WINDOWS
            timeval time;

            time.tv_sec = timeout / 1000000;
            time.tv_usec = timeout - time.tv_sec * 1000000;

            FD_SET(this->_descriptor, &this->_readingSelector);
            FD_SET(this->_descriptor, &this->_writingSelector);
            FD_SET(this->_descriptor, &this->_exceptionSelector);

            int tmp = ::select(0, &this->_readingSelector, &this->_writingSelector, &this->_exceptionSelector, &time);

            if(tmp == SOCKET_ERROR)
                result = false;

            else
            {
                if(tmp > 0)
                {
                    reading = FD_ISSET(this->_descriptor, &this->_readingSelector);
                    writing = FD_ISSET(this->_descriptor, &this->_writingSelector);
                    exception = FD_ISSET(this->_descriptor, &this->_exceptionSelector);

                    return true;
                }

                result = true;
            }
#endif // WINDOWS
        }

        reading = false;
        writing = false;
        exception = false;

        return result;
    }

    bool TcpSocket::isOpened()
    {
#if SYSTEM == LINUX
        return this->_descriptor != -1;
#endif // LINUX

#if SYSTEM == WINDOWS
        return this->_descriptor != INVALID_SOCKET;
#endif // WINDOWS
    }

    uint32_t TcpSocket::read(const uint8_t *buffer, uint32_t length)
    {
        if(this->isOpened())
        {
            uint32_t offset = 0;

            while(length > 0)
            {
                uint32_t count = (length > 256 ? 256 : length);

#if SYSTEM == LINUX
                int tmp = ::recv(this->_descriptor, (void *)buffer + offset, count, 0);
#endif

#if SYSTEM == WINDOWS
                int tmp = ::recv(this->_descriptor, (char *)buffer + offset, count, 0);
#endif
                if(tmp == 0)
                    return offset;

                if(tmp == (-1))
                    return (offset > 0 ? offset : Error);

                offset += tmp;
                length -= tmp;
            }

            return offset;
        }

        return Error;
    }

    uint32_t TcpSocket::write(const uint8_t *buffer, uint32_t length)
    {
        if(this->isOpened())
        {
            uint32_t offset = 0;

            while(length > 0)
            {
                uint32_t count = (length > 256 ? 256 : length);

#if SYSTEM == LINUX
                int tmp = ::send(this->_descriptor, (void *)buffer + offset, count, 0);
#endif

#if SYSTEM == WINDOWS
                int tmp = ::send(this->_descriptor, (char *)buffer + offset, count, 0);
#endif
                if(tmp == 0)
                    return offset;

                if(tmp == (-1))
                    return (offset > 0 ? offset : Error);

                offset += tmp;
                length -= tmp;
            }

            return offset;
        }

        return Error;
    }

    uint32_t TcpSocket::getInputDataSize()
    {
        if(this->isOpened())
        {
#if SYSTEM == LINUX
            int count = 0;

            if (::ioctl(this->_descriptor, FIONREAD, &count) == -1)
                return 0;
#endif

#if SYSTEM == WINDOWS
            u_long count = 0;

            if (::ioctlsocket(this->_descriptor, FIONREAD, &count) == SOCKET_ERROR)
                return 0;
#endif

            return (uint32_t)count;
        }

        return 0;
    }
}}}
