#pragma once
#include "Interactables\BoxyBox.h"

class TileMenuHandler : public sf::Drawable
{
public:
    TileMenuHandler();
    virtual ~TileMenuHandler() {};

    void update();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    BoxyBox tileBox;
    int activeTileTexture;

    sf::Vector2f offset;
};
