#pragma once

#include "observer.h"
#include "memory"


class ObserverFactory
{
public:
    ObserverFactory(std::string_view obs_folder);
    ObserverFactory(const Observer &) = delete;
    ObserverFactory& operator=(const Observer &) = delete;

    bool selectProcess(std::string_view name);
    std::shared_ptr<Observer> makeObserver(const std::string& name, void* address) const;

private:
    int _pid = -1;
    std::string_view _obs_folder;

};
