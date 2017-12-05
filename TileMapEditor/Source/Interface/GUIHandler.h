#pragma once
#include "SFML\Window\Window.hpp"
#include "Interactables\ImgButton.h"
#include <unordered_map>


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

    std::unordered_map<std::string, ImgButton> imgButtons;

    tgui::Gui gui;
    bool active;
};