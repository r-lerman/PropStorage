#pragma once
#include "PropertiesStorage.h"

class Console
{
public:
    
    Console(Storage::PropertyStorage& s) : storage(s) { }

    void Run(std::istream& in, std::ostream& out);

private:

    void ProcessCommand(std::ostream& out, std::string cmn_name, std::string cmn_text);

    Storage::PropertyStorage &storage;
};
