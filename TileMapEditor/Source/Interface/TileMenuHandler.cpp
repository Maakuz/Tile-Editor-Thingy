#include "TileMenuHandler.h"
#include "Queues\TileQueue.h"
#include "Queues\OverlayQueue.h"
#include "Queues\BackgroundQueue.h"
#include "Structs.h"
#include "Constants.h"
#include "SFML\Window\Mouse.hpp"
#include "GUI.h"


TileMenuHandler::TileMenuHandler() :
    tileBox(TILEMENU_BORDER_SIZE, TILEMENU_BORDER_SIZE + MENU_BAR_HEIGHT, DEFAULT_TILE_SIZE * TILEMENU_X_AREA, DEFAULT_TILE_SIZE * TILEMENU_Y_AREA, TILEMENU_BORDER_SIZE)
{
    tileBox.setColor(sf::Color(0, 100, 200));
    tileBox.setBorderColor(sf::Color(100, 100, 100));
    activeTileTexture = 0;
    offset.x = TILEMENU_BORDER_SIZE;
    offset.y = TILEMENU_BORDER_SIZE + MENU_BAR_HEIGHT;
    
    createTileButtons();

    Global::gui->get<tgui::MenuBar>(Global::Elements::Menu::bar)->connect("MenuItemClicked", &TileMenuHandler::handleFileMenu, this);
    blockDrawing = false;
}

void TileMenuHandler::handleEvent(sf::Event event)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            pressedPos = { event.mouseButton.x - offset.x, event.mouseButton.y - offset.y };

            if (tileBox.contains(pressedPos.x, pressedPos.y))
                blockDrawing = true;
        }
        break;


    case sf::Event::MouseButtonReleased:

        if (event.mouseButton.button == sf::Mouse::Left)
        {
            releasedPos = { event.mouseButton.x - offset.x, event.mouseButton.y - offset.y };

            handleBlockSelection();
            blockDrawing = false;
        }
        break;
    }
}

void TileMenuHandler::update(sf::Vector2i mousePos)
{
    if (!blockDrawing)
        layerManager.update(activeTileTexture, activeTiles, mousePos);
}

void TileMenuHandler::queueItems()
{
    layerManager.queueTiles();


    for (int i = 0; i < TILEMENU_Y_AREA; i++)
    {
        for (int j = 0; j < TILEMENU_X_AREA; j++)
        {
            Tile activeTile;
            activeTile.textureID = activeTileTexture;
            activeTile.tileID = j + TILEMENU_X_AREA * i;
            activeTile.x = j * DEFAULT_TILE_SIZE + offset.x;
            activeTile.y = i * DEFAULT_TILE_SIZE + offset.y;

            TileQueue::get().queue(activeTile);
        }
    }

    BackgroundQueue::get().queue(tileBox.getRect());

    for each (ActiveTile i in activeTiles)
    {
        OverlayQueue::get().queue(buttons[i.id]);
    }
}

void TileMenuHandler::createTileButtons()
{
    buttons.resize(TILEMENU_Y_AREA * TILEMENU_X_AREA);

    for (int i = 0; i < TILEMENU_Y_AREA; i++)
    {
        for (int j = 0; j < TILEMENU_X_AREA; j++)
        {
            buttons[j + TILEMENU_X_AREA * i].setPosition(j * DEFAULT_TILE_SIZE + offset.x, i * DEFAULT_TILE_SIZE + offset.y);
            buttons[j + TILEMENU_X_AREA * i].setOutlineThickness(-2);
            buttons[j + TILEMENU_X_AREA * i].setSize({ DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE });
            buttons[j + TILEMENU_X_AREA * i].setFillColor(sf::Color::Transparent);
        }
    }
}

void TileMenuHandler::handleFileMenu(sf::String button)
{
    if (button == Global::Elements::Menu::Clickables::saveFile)
        fileManager.save(layerManager);

    if (button == Global::Elements::Menu::Clickables::openFile)
        fileManager.load(layerManager);

    if (button == Global::Elements::Menu::Clickables::exportLayers)
        fileManager.exportTextures(layerManager);
}

void TileMenuHandler::handleBlockSelection()
{
    bool cleared = false;

    
    pressedPos /= DEFAULT_TILE_SIZE;
    releasedPos /= DEFAULT_TILE_SIZE;


    if (pressedPos.x > releasedPos.x)
        std::swap(pressedPos.x, releasedPos.x);

    if (pressedPos.y > releasedPos.y)
        std::swap(pressedPos.y, releasedPos.y);


    for (int k = pressedPos.y; k <= releasedPos.y; k++)
    {
        for (int j = pressedPos.x; j <= releasedPos.x; j++)
        {
            for (size_t i = 0; i < buttons.size(); i++)
            {

                if (buttons[i].isInside(j * DEFAULT_TILE_SIZE + offset.x, k * DEFAULT_TILE_SIZE + offset.y))
                {
                    if (!cleared)
                    {
                        for (ActiveTile i : activeTiles)
                        {
                            buttons[i.id].setOutlineColor(sf::Color::Transparent);
                        }
                        activeTiles.clear();

                        cleared = true;
                    }

                    ActiveTile activeTile;
                    activeTile.id = i;
                    activeTile.x = j;
                    activeTile.y = k;

                    activeTiles.push_back(activeTile);
                    buttons[i].setOutlineColor(sf::Color::White);
                }
            }
        }
    }
}
