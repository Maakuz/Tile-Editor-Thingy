#pragma once
#include "TGUI/TGUI.hpp"

class ImgButton
{
public:
    ImgButton(sf::Texture pressedTex, sf::Texture unpressedTex);
    virtual ~ImgButton() {};

    //The object will probably work but textures are recommended
    ImgButton();
   

    void swap();

    tgui::Widget::Ptr getButton() { return button; };

    //returns true if pressed
    operator bool() const { return pressed; };

    operator tgui::Widget::Ptr() const { return button; };
    operator tgui::Button::Ptr() const { return button; };
private:
    tgui::Button::Ptr button;
    sf::Texture pressedTex;
    sf::Texture unpressedTex;
    bool pressed;
};