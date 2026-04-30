#pragma once

#include <string>
#include <sys/uio.h>

class Observer
{
public:
    Observer(int pid, void* address, const std::string out_file);

    Observer(const Observer &) = delete;
    Observer& operator=(const Observer &) = delete;

    bool update() const;

private:
    int _pid;
    void* _address;
    const std::string _path;

    int _ret;
    const iovec _local = {.iov_base = &_ret, .iov_len = sizeof(_ret)};
    iovec _remote;

};
