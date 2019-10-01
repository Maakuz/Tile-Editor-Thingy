#include "TileManager.h"
#include "SFML\Graphics.hpp"
#include "Constants.h"
#include "Queues\TileQueue.h"
#include "TileMaps.h"

sf::Color HITBOX_COLORS[2] = { sf::Color(0, 0, 0, 150), sf::Color(255, 255, 0, 150) };

TileManager::TileManager()
{
}

TileManager::~TileManager()
{
    for (size_t i = 0; i < spriteQueue.size(); i++)
        delete spriteQueue[i];
}

void TileManager::prepareTiles()
{
    for (size_t i = 0; i < spriteQueue.size(); i++)
        delete spriteQueue[i];

    spriteQueue.clear();

    for (Tile tile : TileQueue::get().getQueue())
    {
        if (tile.tileID < HITBOX_ID_START)
        {
            sf::Sprite* sprite = new sf::Sprite;
            sprite->setTexture(TileMaps::get().getTexture(tile.textureID));
            sprite->setPosition((float)tile.x, (float)tile.y);
            sprite->setTextureRect(TileMaps::get().getTileRect(tile.textureID, tile.tileID));
            sprite->setColor(tile.color);
            spriteQueue.push_back(sprite);
        }

        else if (tile.tileID > HITBOX_ID_START)
        {
            sf::RectangleShape* rect = new sf::RectangleShape;
            rect->setPosition((float)tile.x, (float)tile.y);
            rect->setSize(sf::Vector2f(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE));
            rect->setFillColor(HITBOX_COLORS[tile.tileID - HITBOX_ID_START - 1]);
            spriteQueue.push_back(rect);
        }

    }
}

void TileManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    for (size_t i = 0; i < spriteQueue.size(); i++)
    {
        target.draw(*spriteQueue[i], states);
    }
}
