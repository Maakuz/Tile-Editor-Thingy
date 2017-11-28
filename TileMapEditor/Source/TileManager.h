#pragma once
#include <vector>
#include <unordered_map>
#include "SFML\Graphics.hpp"
#include "Structs.h"

class TileManager : public sf::Drawable
{
public:
    TileManager();
    virtual ~TileManager() {};

    void loadTextures();

    void queueTile(Tile tile);
    void clearQueue();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::vector<sf::Texture> textures;
    sf::Sprite test;
    sf::CircleShape test2;

    std::unordered_map<int, std::vector<Tile>> tileQueue;
};