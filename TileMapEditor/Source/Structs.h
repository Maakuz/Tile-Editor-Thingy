#pragma once
#include <vector>
#include "Interactables\Button.h"

struct Tile
{
    Tile()
    {
        color = sf::Color::White;
    };
    int x;
    int y;
    //What texture.
    int textureID;
    //Where in the texture.
    int tileID;

    //an overlay color
    sf::Color color;
};

//Coordianates are in relation to each other in tile-space...?
struct ActiveTile
{
    ActiveTile(Button::SIDES::SIDES_VISIBLE visibleSide = Button::SIDES::SIDES_VISIBLE::NONE)
    {
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineColor(sf::Color::White);
        box.setVisibleSide(visibleSide);
        box.setOutlineThickness(-2);
    };

    Button box;
    int tileID;
    int textureID;
    int x;
    int y;
};