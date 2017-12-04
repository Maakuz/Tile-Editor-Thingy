#pragma once
#include "SFML\System\String.hpp"
#include <filesystem>

namespace fs = std::experimental::filesystem;



class LoadWindow
{
public:
    LoadWindow();
    virtual ~LoadWindow() {};

    bool isLoading() const { return loading; };
    void openWindow();
    void closeWindow();
    fs::path getPath() const { return currentDir; };

private:
    void printPaths();
    void iteratePaths(sf::String name, sf::String path);

    bool loading;
    fs::path currentDir;
};