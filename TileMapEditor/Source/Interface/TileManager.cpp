#include "TileManager.h"
#include "SFML\Graphics.hpp"
#include "Constants.h"
#include "Queues\TileQueue.h"
#include "TileMaps.h"

TileManager::TileManager()
{
}

void TileManager::prepareTiles()
{
    spriteQueue.clear();

    for (Tile tile : TileQueue::get().getQueue())
    {
        sf::Sprite sprite;
        sprite.setTexture(TileMaps::get().getTexture(tile.textureID));
        sprite.setPosition((float)tile.x, (float)tile.y);
        sprite.setTextureRect(TileMaps::get().getTileRect(tile.textureID, tile.tileID));
        sprite.setColor(tile.color);

        spriteQueue.push_back(sprite);
    }
}

void TileManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    for (size_t i = 0; i < spriteQueue.size(); i++)
    {
        target.draw(spriteQueue[i], states);
    }
}
