#pragma once
#include <vector>
#include "SFML\Graphics.hpp"
#include "Structs.h"

class TileManager : public sf::Drawable
{
public:
    TileManager();
    virtual ~TileManager() {};


    void prepareTiles();


private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void loadTextures();

    std::vector<sf::Texture> textures;
    std::vector<std::vector<sf::IntRect>> tileRects;

    std::vector<sf::Sprite> spriteQueue;
};