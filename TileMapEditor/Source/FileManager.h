#pragma once
#include "Interface\LayerManager.h"

class FileManager
{
public:
    FileManager();
    virtual ~FileManager() {}
    

    void save(const LayerManager & layerManager) const;

    void load(LayerManager & layerManager);

private:

};

