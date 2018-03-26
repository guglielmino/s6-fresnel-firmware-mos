//
// Created by Fabrizio Guglielmino on 22/10/17.
//

#pragma once

#include <functional>

class IUART {
public:
    std::function<void ()> _handler_func;

    virtual size_t write(const void* buffer, size_t len) = 0;
    virtual void flush() = 0;
    virtual size_t read(void *buf, size_t len)  = 0;
    virtual void readAsync(std::function<void ()> cb)  = 0;
    virtual size_t readAvail()  = 0;
    virtual size_t writeAvail() = 0;
};