#pragma once
#include "SFML\Window\Window.hpp"
#include <unordered_map>
#include "GUI.h"


class GUIHandler
{
public:
    GUIHandler(sf::RenderWindow & window);
    virtual ~GUIHandler() {};

    void handleEvents(sf::Event event);

    void update();

    bool isActive() { return active; };

    void drawGui();

    //It does what it's supposed to.
    void crashEverything();

private:
    tgui::Gui gui;
    bool active;
};