#ifndef MAPPEDBUFFER_H
#define MAPPEDBUFFER_H

#include <iostream>
#include <windows.h>
#include "SharedFileOut.h"

template <typename MappedBufferT>
class MappedBuffer
{
private:
    HANDLE file = nullptr;
    MappedBufferT *buffer = nullptr;
    std::string BUFFER_NAME;
    size_t BUFFER_SIZE_BYTES;

public:
    MappedBuffer(std::string name);
    void create();
    void open();
    MappedBufferT *view();
    void write();
    void write(int m_id);
    void close();
};

#include "MappedBuffer.cpp"

#endif