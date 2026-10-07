#include "lib/observer_factory.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <thread>
#include <chrono>
#include <map>

struct Game
{
    const std::string_view name;
    const std::string_view process_name;
    std::map<const std::string, void*> values;
};

const std::vector<Game> games = {
    {"DarkSoulsRemastered" , "DarkSoulsRemast", {{"deaths_all", (void*)0x9828998}}},
    {"DarkSouls2sin" , "DarkSoulsII.exe", {{"deaths_all", (void*)0x7fffeb71f124}}},
    {"DarkSouls3" , "DarkSoulsIII.ex", {{"deaths_all", (void*)0x7fff1bd018e8}, {"death_session", (void*)0x7fff1bd18598}}},
};

static void printHelp()
{
    std::cout << "\n\nexample: sudo ./DarkCounter <number>\nThe following arguments are supported:\n";
    for(size_t i = 0; i < games.size(); ++i)
    {
        std::cout << "\t" << i << " : " << games[i].name << "\n";
    }
    std::cout << "\n";
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

    
    size_t index = SIZE_MAX;
    try {
        index = std::stoi(argv[1]);
    } catch (std::invalid_argument) {
        std::cout << "Invalid argument";
        printHelp();
        return -1;
    }
    if(index >= games.size())
    {
        std::cout << "Unknown index argument: " << index;
        printHelp();
        return 0;
    }

    const auto& game = games[index];
    if(!factory.selectProcess(game.process_name))
    {
        std::cout << "Could not find a process with name: " << game.process_name << "\nGame needs to be running when starting the counter\n\n";
        return -1; 
    }

    std::vector<std::shared_ptr<Observer>> observers;
    observers.reserve(game.values.size());

    std::cout << "Creating observers\n";
    for(const auto& [name, address] : game.values)
    {
        observers.push_back(factory.makeObserver(name, address));
        std::cout << "\tCreated observer for: " << name << "\n";
    }

    std::cout << "Starting counting\n";
    while(true)
    {
        for(const auto& obs : observers)
        {
            if(!obs->update())
            {
                std::cout << "Failed to update observer\n";
                return -1;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}