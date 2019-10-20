#pragma once
#include <SFML/Graphics.hpp>
#include "TileMenuHandler.h"
#include "Renderer.h"


class Editor
{
public:
    Editor(sf::RenderWindow & window);
    virtual ~Editor() {};

    int run(sf::RenderWindow & window);
private:
    void scrollScreen(float dt);

    TileMenuHandler tileMenuHandler;
    Renderer renderer;
    sf::View workView;
    sf::View toolView;
    sf::Clock deltaTimer;

};