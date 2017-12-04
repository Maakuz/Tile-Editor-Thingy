#pragma once
#include "SFML\System\String.hpp"
#include <filesystem>

namespace fs = std::experimental::filesystem;

class SaveWindow
{
public:
    SaveWindow();
    virtual ~SaveWindow() {};

    bool isSaving() const { return saving; };
    void openWindow();
    void closeWindow();
    fs::path getPath() const;
private:
    void printPaths();
    void iteratePaths(sf::String name, sf::String path);
    void showFolderCreationWindow();
    void cancelFolderCreation();
    void createFolder();

    bool saving;
    bool creatingFolder;
    fs::path currentDir;
};