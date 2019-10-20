#pragma once
#include "Interface\LayerManager.h"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class FileManager
{
public:
    FileManager();
    virtual ~FileManager() {}
    

    void save(const LayerManager & layerManager, fs::path path) const;
    void quickSave(const LayerManager & layerManager) const;
    void load(LayerManager & layerManager, fs::path path);

    void exportTextures(const LayerManager & layerManager);
    std::vector<fs::path> getTexturePaths();
    int addTexture(std::string name, std::string path);
private:
    fs::path quickSavePath;
};

