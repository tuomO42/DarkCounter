#include "observer.h"

#include <fstream>
#include <format>
#include <iostream>

#include <errno.h>
#include <string.h>


Observer::Observer(int pid, void* address, std::string_view out_file):
_pid(pid),
_address(address),
_path(out_file)
{
    _remote = {.iov_base = _address, .iov_len = sizeof(int)};
}

bool Observer::update() const
{
    if(process_vm_readv(_pid, &_local, 1, &_remote, 1, 0) < 0)
    {
        std::cout << "Failed to read from the process: " << strerror(errno) << "\n";
        return false;
    }

    std::ofstream out(_path.data());
    auto val = std::format("{}", _ret);
    out.write(val.c_str(), val.size());

    return true;
}