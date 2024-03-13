#include "MappedBuffer.h"

template <typename MappedBufferT>
MappedBuffer<MappedBufferT>::MappedBuffer(std::string name) {
    BUFFER_NAME = name;
    BUFFER_SIZE_BYTES = sizeof(MappedBufferT);
}

template <typename MappedBufferT>
void MappedBuffer<MappedBufferT>::create()
{
    file = CreateFileMapping(
        INVALID_HANDLE_VALUE, // use paging file
        NULL,                 // default security
        PAGE_READWRITE,       // read/write access
        0,                    // maximum object size (high-order DWORD)
        BUFFER_SIZE_BYTES,    // maximum object size (low-order DWORD)
        BUFFER_NAME.c_str()); // name of mapping object

    if (file == nullptr)
        throw std::runtime_error("Couldn't create file mapping (" + BUFFER_NAME + ").");

    // message identifier -1 on create
    write(-1);
}

template <typename MappedBufferT>
void MappedBuffer<MappedBufferT>::open()
{
    file = OpenFileMappingA(
        FILE_MAP_ALL_ACCESS,  // read/write access
        FALSE,                // do not inherit the name
        BUFFER_NAME.c_str()); // name of mapping object

    if (file == nullptr)
        throw std::runtime_error("Couldn't open file mapping (" + BUFFER_NAME + ").");
}

template <typename MappedBufferT>
MappedBufferT *MappedBuffer<MappedBufferT>::view()
{
    if (buffer != nullptr)
        return buffer;

    buffer = (MappedBufferT *)MapViewOfFile(file, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE_BYTES);

    if (buffer == nullptr)
        throw std::runtime_error("Couldn't map view of file (" + BUFFER_NAME + ").");

    return buffer;
}

template <typename MappedBufferT>
void MappedBuffer<MappedBufferT>::write()
{
    if (buffer == nullptr) view();

    BaseInfo_t *baseBuffer = (BaseInfo_t *)buffer;
    baseBuffer->m_id++;

    if (!FlushViewOfFile(buffer, BUFFER_SIZE_BYTES))
        throw std::runtime_error("Couldn't flush view of file (" + BUFFER_NAME + ").");
}

template <typename MappedBufferT>
void MappedBuffer<MappedBufferT>::write(int m_id)
{
    if (buffer == nullptr) view();

    BaseInfo_t *baseBuffer = (BaseInfo_t *)buffer;
    baseBuffer->m_id = m_id;

    if (!FlushViewOfFile(buffer, BUFFER_SIZE_BYTES))
        throw std::runtime_error("Couldn't flush view of file (" + BUFFER_NAME + ").");
}

template <typename MappedBufferT>
void MappedBuffer<MappedBufferT>::close()
{
    if (buffer != nullptr)
        UnmapViewOfFile(buffer);

    if (file != nullptr)
        CloseHandle(file);

    buffer = nullptr;
    file = nullptr;
}