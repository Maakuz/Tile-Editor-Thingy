#pragma once
#include "SFML\Window\Window.hpp"
#include "TGUI\TGUI.hpp"


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
    void handleMenuBarClick(sf::String itemClicked);

    tgui::Gui gui;
    bool active;
};