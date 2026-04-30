#include "observer_factory.h"

#include <format>
#include <filesystem>
#include <exception>
#include <vector>
#include <algorithm>
#include <fstream>

#include "iostream"

ObserverFactory::ObserverFactory(std::string_view obs_path)
{
    _obs_folder = obs_path;
    if(!std::filesystem::exists(_obs_folder) && !std::filesystem::create_directory(_obs_folder.data()))
    {
        throw std::runtime_error(std::format("Failed to create output folder: {}", _obs_folder));
    }
}

static std::vector<std::filesystem::path> findAllPids()
{
    constexpr auto proc = "/proc/";
    std::vector<std::filesystem::path> ret;
    ret.reserve(600);

    std::ranges::for_each(
        std::filesystem::directory_iterator(proc),
        [&ret](const auto& dir_ent){
            if(!dir_ent.is_directory() || dir_ent.is_symlink())
            {
                return;
            }

            ret.push_back(dir_ent.path());
        }
    );
    
    return ret;
}

bool ObserverFactory::selectProcess(std::string_view name)
{
    if(_pid != -1)
    {
        return false;
    }

    auto pids = findAllPids();
    int result = -1;
    for(const auto& pid : pids){

        auto comm = std::format("{}/comm", pid.string());
        if(!std::filesystem::exists(comm)){
            continue;
        }

        std::ifstream reader(comm);
        std::string ret;
        std::getline(reader, ret);
        
        if(ret == name){
            std::string path = pid;
            auto start = path.find_last_of("/") + 1;
            _pid = std::stoi(path.substr(start, path.length() - start));
            break;
        }
    };

    std::cout << "Selected process with pid: " << _pid << "\n";
    return !(_pid == -1);
}


Observer ObserverFactory::makeObserver(const std::string& name, void* address) const
{
    std::string out_file = std::format("./{}/{}.txt", _obs_folder, name);

    return Observer(_pid, address, out_file);
}