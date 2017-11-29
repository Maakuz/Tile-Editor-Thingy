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

void FileManager::exportTextures(const LayerManager & layerManager)
{
    for (int i = 0; i < LAYER_AMOUNT; i++)
    {
        std::string fileName;
        fileName = "wat" + std::to_string(i) + ".png";
        layerManager.getLayerAsImage(i).saveToFile(fileName);
    }

    printf("exprot comprot\n");
}
