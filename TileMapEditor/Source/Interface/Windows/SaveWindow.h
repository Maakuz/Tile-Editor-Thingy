#pragma once
#include "SFML\System\String.hpp"
#include <filesystem>

namespace fs = std::filesystem;

class SaveWindow
{
public:
    SaveWindow();
    virtual ~SaveWindow() {};

    bool isOpen() const { return open; };
    void openWindow();
    void closeWindow();
    fs::path getPath() const;
private:
    void printPaths();
    void iteratePaths(sf::String name, sf::String path);
    void showFolderCreationWindow();
    void cancelFolderCreation();
    void createFolder();

    bool open;
    bool creatingFolder;
    fs::path currentDir;
};