#include "lib/observer_factory.h"

#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>

struct MemValue
{
    const std::string name;
    void* address;
};

const std::unordered_map<std::string_view, const MemValue> games = {
    {"bash", {"random", (void*)std::stoul("7fcd7dd8b000", nullptr, 16)}},

};

static void printHelp()
{
    std::cout << "\n\nexample: sudo ./DarkCounter <game>\nThe following arguments are supported:\n";
    for(const auto& [name, mem] : games)
    {
        std::cout << "\t" << name << "\n";
    }
}

int main(int argc, char* argv[])
{
    auto factory = ObserverFactory("values");

    if(argc < 2)
    {
        std::cout << "Missing game name argument";
        printHelp();
        return 0;
    }

    std::string_view process_name = argv[1];
    auto it = games.find(process_name);
    if(it == games.end())
    {
        std::cout << "Unknown game given as argument: " << process_name;
        printHelp();
        return 0;
    }

    if(!factory.selectProcess(it->first))
    {
        std::cout << "Could not find a process with name: " << it->first << "\n";
        return -1; 
    }

    auto observer = factory.makeObserver(it->second.name, it->second.address);

    while(true)
    {
        if(!observer.update())
        {
            std::cout << "Failed to update observer\n";
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}