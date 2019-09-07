#include "FileManager.h"
#include <fstream>
#include "GUI.h"
#include <Constants.h>
#include "TileMaps.h"

namespace fs = std::experimental::filesystem;

FileManager::FileManager()
{
    this->quickSavePath = fs::current_path() / "Saves/";
}

void FileManager::save(const LayerManager & layerManager, fs::path path) const
{
    path.replace_extension(FILE_EXTENSION);

    std::ofstream file(path);
    if (file.is_open())
    {
        file << layerManager << "\n";
        file << TileMaps::get() << "\n";

        file.close();

        printf("Saved!\n");
    }

    else
        printf("NOT saved!\n");

}

void FileManager::quickSave(const LayerManager & layerManager) const
{
    
    fs::path fullPath = this->quickSavePath;
    fullPath.replace_filename(DEFAULT_QUICKSAVE_FILE_NAME);
    fullPath.replace_extension(FILE_EXTENSION);

    std::ofstream file(fullPath);
    if (file.is_open())
    {
        file << layerManager << "\n";
        file << TileMaps::get() << "\n";

        file.close();

        printf("Quacksaved!\n");
    }

    else
        printf("NOT quacksaved!\n");
}

void FileManager::load(LayerManager & layerManager, fs::path path)
{
    std::ifstream file(path);
    if (file.is_open())
    {
        file >> layerManager;
        file >> TileMaps::get();
        file.close();

        printf("Loaded?!\n");
    }

    else 
        printf("NOT loaded!\n");
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
    currentDir /= TILE_MAP_FOLDER;

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

int FileManager::addTexture(sf::String name, sf::String path)
{
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

    return TileMaps::get().getTextureIndex(name);
}
