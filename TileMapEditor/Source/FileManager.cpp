#include "FileManager.h"
#include <fstream>
#include "GUI.h"

FileManager::FileManager()
{
}

void FileManager::save(const LayerManager & layerManager) const
{
    std::ofstream file("test.txt");

    file << layerManager;

    file.close();

    printf("Saved!\n");

    //Things to save:
    //layers
    //textures
    //Editor state
}

void FileManager::load(LayerManager & layerManager)
{
    std::ifstream file("test.txt");

    file >> layerManager;
    file.close();

    printf("Loaded?!\n");

}
