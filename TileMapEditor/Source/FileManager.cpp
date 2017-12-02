#include "FileManager.h"
#include <fstream>
#include "GUI.h"
#include <filesystem>
#include <Constants.h>
#include "TileMaps.h"

namespace fs = std::experimental::filesystem;

FileManager::FileManager()
{
    Global::gui->get(Global::Elements::textureImporter::textureList)->connect("DoubleClicked", &FileManager::addTexture, this);
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

void FileManager::importTexure()
{
    fs::path currentDir = fs::current_path();
    currentDir /= RESOURCE_FOLDER;

    printf("%ws\n", currentDir.c_str());

    std::vector<fs::path> textures;


    for (auto & p : fs::directory_iterator(currentDir))
        textures.push_back(p);

    auto textureList = Global::gui->get<tgui::ListBox>(Global::Elements::textureImporter::textureList);
    textureList->show();
    textureList->removeAllItems();
    
    for (fs::path & p : textures)
    {
        textureList->addItem(p.filename().string(), p.string());
    }
}

void FileManager::addTexture(sf::String name, sf::String path)
{
    printf("%s\n", std::string(path).c_str());

    try
    {
        TileMaps::get().addTexture(name);
    }
    catch (const char* e)
    {
        printf("%s\n", e);
    }

    Global::gui->get(Global::Elements::textureImporter::textureList)->hide();
    auto textureInfo = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel)->get<tgui::ComboBox>(Global::Elements::infoBox::textureBox);

    textureInfo->addItem(name, path);
}
