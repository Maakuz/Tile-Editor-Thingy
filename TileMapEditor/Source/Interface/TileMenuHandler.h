#pragma once
#include "LayerManager.h"
#include <Interactables\Button.h>
#include <Interactables\BoxyBox.h>
#include "FileManager.h"
#include "Windows/ResizeWindow.h"
#include "Windows/SaveWindow.h"
#include "Windows/LoadWindow.h"
#include <vector>

class TileMenuHandler
{
public:
    TileMenuHandler();
    virtual ~TileMenuHandler() {};

    void handleEvent(sf::Event event, bool guiActive, sf::Vector2i viewPortMousePos);
    void update(sf::Vector2i mousePos, sf::Vector2i viewPortMousePos, bool guiActive);

    void queueItems(sf::View viewArea);

    std::vector<ActiveTile> getActiveTile() { return activeTiles; };
    int getActiveTexture() { return activeTileTexture; };

    void autosave();

private:
    void handleMouseEvents(sf::Event event, bool guiBlock, sf::Vector2i viewPortMousePos);
    void handleKeyboardEvents(sf::Event event);

    void createTileButtons();
    void handleMenuBar();
    void handleHelpWindow();
    void handleBlockSelection(sf::Vector2i start, sf::Vector2i stop);
    void handleLayerSelection(sf::Vector2i start, sf::Vector2i stop);
    void createActiveBounds(std::vector<ActiveTile>& tiles);
    void swapStartAndStopPosition(sf::Vector2i & start, sf::Vector2i & stop);
    void setActiveTexture(sf::String name, sf::String path);
    void importTexture(sf::String name, sf::String path);
    void equipEraser();
    bool anyWindowsOpen();

    void updateInfoBox();


    void resize();
    void saveFile();
    void loadFile();


    FileManager fileManager;
    LayerManager layerManager;
    SaveWindow saveWindow;
    LoadWindow loadWindow;
    ResizeWindow resizeWindow;
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
