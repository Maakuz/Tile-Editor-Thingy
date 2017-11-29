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
    activeTile = 0;
    offset.x = TILEMENU_BORDER_SIZE;
    offset.y = TILEMENU_BORDER_SIZE + MENU_BAR_HEIGHT;
    
    createTileButtons();

    Global::gui->get<tgui::MenuBar>(Global::Elements::Menu::bar)->connect("MenuItemClicked", &TileMenuHandler::handleFileMenu, this);

}

void TileMenuHandler::update(const sf::RenderWindow & window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    layerManager.update(activeTileTexture, activeTile, mousePos);


    for (size_t i = 0; i < buttons.size(); i++)
    {
        if (buttons[i].isInside(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            buttons[activeTile].setOutlineColor(sf::Color::Transparent);
            buttons[i].setOutlineColor(sf::Color::White);
            activeTile = i;
        }
    }

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
    OverlayQueue::get().queue(buttons[activeTile]);
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
}