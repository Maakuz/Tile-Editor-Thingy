#include "TileManager.h"
#include "SFML\Graphics.hpp"
#include "Constants.h"
#include "Queues\TileQueue.h"

TileManager::TileManager()
{
    loadTextures();
}

void TileManager::loadTextures() // TODO: MAKE THIS FOR LOADING DURING RUNTIME //
{
    sf::Texture tileTexture;
    tileTexture.loadFromFile(TEXTURE_PATH("test.png"));

    textures.push_back(tileTexture);

    int x = tileTexture.getSize().x / DEFAULT_TILE_SIZE;
    int y = tileTexture.getSize().y / DEFAULT_TILE_SIZE;

    //initialize with the size it needs
    tileRects.push_back(std::vector<sf::IntRect>(x * y));

    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            tileRects[0][j + (i * x)] = sf::IntRect(
                j * DEFAULT_TILE_SIZE,
                i * DEFAULT_TILE_SIZE,
                DEFAULT_TILE_SIZE,
                DEFAULT_TILE_SIZE);
        }
    }
}

void TileManager::prepareTiles()
{
    spriteQueue.clear();

    for (Tile tile : TileQueue::get().getQueue())
    {
        sf::Sprite sprite;
        sprite.setTexture(textures[tile.textureID]);
        sprite.setPosition(tile.x, tile.y);
        sprite.setTextureRect(tileRects[tile.textureID][tile.tileID]);

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
