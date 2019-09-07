#pragma once
#include "Interface\LayerManager.h"
#include <filesystem>

namespace fs = std::experimental::filesystem;

class FileManager
{
public:
    FileManager();
    virtual ~FileManager() {}
    

    void save(const LayerManager & layerManager, fs::path path) const;
    void quickSave(const LayerManager & layerManager) const;
    void load(LayerManager & layerManager, fs::path path);

    void exportTextures(const LayerManager & layerManager);
    void importTexure();
    int addTexture(sf::String name, sf::String path);

private:
    fs::path quickSavePath;
    
};

