#include "lib/observer_factory.h"

#include <vector>
#include <iostream>

int main(int argc, char* argv[])
{
    auto factory = ObserverFactory("values");

    std::string_view process_name = argv[1];
    if(!factory.selectProcess(process_name))
    {
        std::cout << "Could not find a process with name: " << process_name << "\n";
        return -1; 
    }

    auto observer = factory.makeObserver("random", (void*)0x078700);

    if(!observer.update())
    {
        std::cout << "Failed to update observer\n";
    }
}