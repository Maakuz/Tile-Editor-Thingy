#pragma once
#include <SFML/Graphics.hpp>
#include "TileManager.h"
#include "TileMenuHandler.h"

class Editor
{
public:
    Editor();
    virtual ~Editor() {};

    int run(sf::RenderWindow & window);
private:
    TileManager tileManager;
    TileMenuHandler tileMenuHandler;

};