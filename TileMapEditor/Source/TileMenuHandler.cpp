#include "TileMenuHandler.h"
#include "Queues\TileQueue.h"
#include "Queues\OverlayQueue.h"
#include "Queues\BackgroundQueue.h"
#include "Structs.h"
#include "Constants.h"
#include "SFML\Window\Mouse.hpp"
#define X_AREA 5
#define Y_AREA 20


TileMenuHandler::TileMenuHandler() :
    tileBox(8, 8, DEFAULT_TILE_SIZE * X_AREA, DEFAULT_TILE_SIZE * Y_AREA, 8)
{
    tileBox.setColor(sf::Color(0, 100, 200));
    tileBox.setBorderColor(sf::Color(100, 100, 100));
    activeTileTexture = 0;
    activeTile = 0;
    offset.x = 8;
    offset.y = 8;

    createTileButtons();
}

void TileMenuHandler::update(const sf::RenderWindow & window)
{
    for (int i = 0; i < Y_AREA; i++)
    {
        for (int j = 0; j < X_AREA; j++)
        {
            Tile activeTile;
            activeTile.textureID = activeTileTexture;
            activeTile.tileID = j + X_AREA * i;
            activeTile.x = j * DEFAULT_TILE_SIZE + offset.x;
            activeTile.y = i * DEFAULT_TILE_SIZE + offset.y;

            TileQueue::get().queue(activeTile);
        }
    }

    BackgroundQueue::get().queue(tileBox.getRect());


    for (size_t i = 0; i < buttons.size(); i++)
    {
        if (buttons[i].isInside(sf::Mouse::getPosition(window)) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            buttons[activeTile].setOutlineColor(sf::Color::Transparent);
            buttons[i].setOutlineColor(sf::Color::White);
            activeTile = i;
        }
    }

    OverlayQueue::get().queue(buttons[activeTile]);
}   

void TileMenuHandler::createTileButtons()
{
    buttons.resize(Y_AREA * X_AREA);

    for (int i = 0; i < Y_AREA; i++)
    {
        for (int j = 0; j < X_AREA; j++)
        {
            buttons[j + X_AREA * i].setPosition(j * DEFAULT_TILE_SIZE + offset.x, i * DEFAULT_TILE_SIZE + offset.y);
            buttons[j + X_AREA * i].setOutlineThickness(-2);
            buttons[j + X_AREA * i].setSize({ DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE });
            buttons[j + X_AREA * i].setFillColor(sf::Color::Transparent);
        }
    }
}
