#include "TileMenuHandler.h"
#include "TileQueue.h"
#include "Structs.h"
#include "Constants.h"
#define X_AREA 5
#define Y_AREA 20


TileMenuHandler::TileMenuHandler() :
    tileBox(8, 8, DEFAULT_TILE_SIZE * X_AREA, DEFAULT_TILE_SIZE * Y_AREA, 8)
{
    tileBox.setColor(sf::Color(0, 100, 200));
    tileBox.setBorderColor(sf::Color(100, 100, 100));
    activeTileTexture = 0;
    offset.x = 8;
    offset.y = 8;
}

void TileMenuHandler::update()
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
}

void TileMenuHandler::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(tileBox, states);
}
