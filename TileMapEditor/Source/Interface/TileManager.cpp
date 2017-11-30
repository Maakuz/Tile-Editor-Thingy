#include "TileManager.h"
#include "SFML\Graphics.hpp"
#include "Constants.h"
#include "Queues\TileQueue.h"
#include "Resources.h"

TileManager::TileManager()
{
    loadTextures();
}

void TileManager::loadTextures() // TODO: MAKE THIS FOR LOADING DURING RUNTIME //
{
    Resources::get().addTexture("grass.png");

    sf::Texture tileTexture = Resources::get().getTexture(0);

    
}

void TileManager::prepareTiles()
{
    spriteQueue.clear();

    for (Tile tile : TileQueue::get().getQueue())
    {
        sf::Sprite sprite;
        sprite.setTexture(Resources::get().getTexture(tile.textureID));
        sprite.setPosition((float)tile.x, (float)tile.y);
        sprite.setTextureRect(Resources::get().getTileRect(tile.textureID, tile.tileID));

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
