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

    void update(const sf::RenderWindow & window);

    void queueItems();

    int getActiveTile() { return activeTile; };
    int getActiveTexture() { return activeTileTexture; };



private:
    void createTileButtons();
    void handleFileMenu(sf::String button);


    FileManager fileManager;
    LayerManager layerManager;
    BoxyBox tileBox;
    int activeTileTexture;
    int activeTile;

    sf::Vector2f offset;

    std::vector<Button> buttons;
};
