#include "TileManager.h"
#include "SFML\Graphics.hpp"
#include "Constants.h"

TileManager::TileManager()
{
    loadTextures();
}

void TileManager::loadTextures()
{
    sf::Texture tileTexture;
    tileTexture.loadFromFile(TEXTURE_PATH("test.png"));

    textures.push_back(tileTexture);
    test.setTexture(textures[0]);
}

void TileManager::queueTile(Tile tile)
{
    tileQueue[tile.textureID].push_back(tile);
}

void TileManager::clearQueue()
{
    tileQueue.clear();
}

void TileManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(test);
}
