#pragma once
#include "LayerManager.h"
#include <Interactables\Button.h>
#include <Interactables\BoxyBox.h>
#include "FileManager.h"
#include "SaveWindow.h"
#include "LoadWindow.h"
#include <vector>

class TileMenuHandler
{
public:
    TileMenuHandler();
    virtual ~TileMenuHandler() {};

    void handleEvent(sf::Event event, bool guiBlock);
    void update(sf::Vector2i mousePos);

    void queueItems();

    std::vector<ActiveTile> getActiveTile() { return activeTiles; };
    int getActiveTexture() { return activeTileTexture; };


private:
    void createTileButtons();
    void handleFileMenu(sf::String button);
    void handleBlockSelection(sf::Vector2i start, sf::Vector2i stop);
    void handleLayerSelection(sf::Vector2i start, sf::Vector2i stop);
    void createActiveBounds(std::vector<ActiveTile>& tiles);
    void swapStartAndStopPosition(sf::Vector2i & start, sf::Vector2i & stop);
    void setActiveTexture(sf::String name, sf::String path);
    void importTexture(sf::String name, sf::String path);
    void equipEraser();
    bool anyWindowsOpen();

    void saveFile();
    void loadFile();

    FileManager fileManager;
    LayerManager layerManager;
    SaveWindow saveWindow;
    LoadWindow loadWindow;
    BoxyBox tileBox;
    int activeTileTexture;
    std::vector<ActiveTile> activeTiles;
    std::vector<ActiveTile> copiedTiles;

    sf::Vector2i offset;

    sf::Vector2i pressedPos;

    std::vector<Button> buttons;

    bool selectingBlocks;
    bool rightClicking;
    bool erasing;
};
