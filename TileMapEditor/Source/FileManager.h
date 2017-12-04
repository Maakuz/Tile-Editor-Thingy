#pragma once
#include "Interface\LayerManager.h"

namespace fs = std::experimental::filesystem;

class FileManager
{
public:
    FileManager();
    virtual ~FileManager() {}
    

    void save(const LayerManager & layerManager, fs::path path) const;

    void load(LayerManager & layerManager, fs::path path);

    void exportTextures(const LayerManager & layerManager);
    void importTexure();
private:
    
    void addTexture(sf::String name, sf::String path);
    
};

