#pragma once
#include <vector>
#include "SFML\Graphics.hpp"
#include "Structs.h"

class TileManager : public sf::Drawable
{
public:
    TileManager();
    virtual ~TileManager();


    void prepareTiles();


private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


    std::vector<sf::Drawable*> spriteQueue;
};