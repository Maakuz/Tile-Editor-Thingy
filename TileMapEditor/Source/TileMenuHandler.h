#pragma once
#include "Interactables\BoxyBox.h"
#include "Interactables\Button.h"
#include <vector>

class TileMenuHandler
{
public:
    TileMenuHandler();
    virtual ~TileMenuHandler() {};

    void update(const sf::RenderWindow & window);

private:

    void createTileButtons();

    BoxyBox tileBox;
    int activeTileTexture;
    int activeTile;

    sf::Vector2f offset;

    std::vector<Button> buttons;
};
