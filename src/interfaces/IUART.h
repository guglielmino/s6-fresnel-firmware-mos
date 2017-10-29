//
// Created by Fabrizio Guglielmino on 22/10/17.
//

#pragma once

class IUART {
public:
    typedef void (*read_async_callback_t)(char *buffer, size_t len);

    virtual size_t write(const void* buffer, size_t len) = 0;
    virtual size_t read(void *buf, size_t len)  = 0;
    virtual void readAsync(read_async_callback_t cb)  = 0;
    virtual size_t readAvail()  = 0;
    virtual size_t writeAvail() = 0;

};