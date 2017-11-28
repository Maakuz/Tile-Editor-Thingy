#pragma once
#include <SFML/Graphics.hpp>
#include "TileManager.h"

class Editor
{
public:
    Editor();
    virtual ~Editor() {};

    int run(sf::RenderWindow & window);
private:
    TileManager tm;


};