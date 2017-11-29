#pragma once
#include "SFML\Graphics.hpp"
#include <Interface\TileManager.h>

class Renderer : public sf::Drawable
{
public:
    Renderer();
    virtual ~Renderer() {};

    void update();
    void clearAllQueues();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    TileManager tileManager;
};