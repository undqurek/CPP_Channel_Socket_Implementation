TARGET = server
VERSION = 1.0.0.0

QT -= core gui
#TEMPLATE = app
#TEMPLATE = lib
CONFIG += console

#DEFINES += CORE_LIBRARY

#CONFIG += thread

#LIBS += -L"$$_PRO_FILE_PWD_/libs/windows/x86/" -l"Ws2_32"
#LIBS += -L"$$_PRO_FILE_PWD_/../core-build-desktop/debug/" -l"core"
#LIBS += -L"$$_PRO_FILE_PWD_/libs/windows/x86-x64/" -l"Ws2_32"

#LIBS += -l"socket" -l"nsl"

HEADERS += \
    basic/preprocesor.h \
    basic/blockingqueue.h \
    basic/mutex.h \
    basic/semaphore.h \
    basic/action.h \
    basic/staticpipe.h \
    basic/dynamicpipe.h \
    network/nonblocking/tcpsocket.h \
    network/nonblocking/tcpclient.h \
    network/nonblocking/tcpserver.h \
    network/channel.h \
    network/parallelsocket.h \
    network/channelextractor.h \

#    network/blocking/tcpclient.h \
#    network/blocking/tcpserver.h \
#    network/blocking/tcpsocket.h

SOURCES += \
    network/channel.cpp \
    network/parallelsocket.cpp \
    basic/mutex.cpp \
    basic/semaphore.cpp \
    network/nonblocking/tcpsocket.cpp \
    network/nonblocking/tcpclient.cpp \
    network/nonblocking/tcpserver.cpp \
    network/channelextractor.cpp \
#    network/blocking/tcpclient.cpp \
#    network/blocking/tcpserver.cpp \
#    network/blocking/tcpsocket.cpp \
    server_main.cpp

