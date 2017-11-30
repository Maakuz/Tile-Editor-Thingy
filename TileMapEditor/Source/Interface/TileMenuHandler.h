#pragma once
#include "LayerManager.h"
#include <Interactables\Button.h>
#include <Interactables\BoxyBox.h>
#include "FileManager.h"
#include <vector>

class TileMenuHandler
{
public:
    TileMenuHandler();
    virtual ~TileMenuHandler() {};

    void handleEvent(sf::Event event);
    void update(sf::Vector2i mousePos);

    void queueItems();

    std::vector<ActiveTile> getActiveTile() { return activeTiles; };
    int getActiveTexture() { return activeTileTexture; };



private:
    void createTileButtons();
    void handleFileMenu(sf::String button);
    void handleBlockSelection();

    FileManager fileManager;
    LayerManager layerManager;
    BoxyBox tileBox;
    int activeTileTexture;
    std::vector<ActiveTile> activeTiles;

    sf::Vector2i offset;

    sf::Vector2i pressedPos;
    sf::Vector2i releasedPos;

    std::vector<Button> buttons;

    bool blockDrawing;
};
