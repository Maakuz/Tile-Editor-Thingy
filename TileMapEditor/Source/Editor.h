#pragma once
#include <SFML/Graphics.hpp>
#include "TileMenuHandler.h"
#include "Renderer.h"
class Editor
{
public:
    Editor();
    virtual ~Editor() {};

    int run(sf::RenderWindow & window);
private:
    TileMenuHandler tileMenuHandler;
    Renderer renderer;

};