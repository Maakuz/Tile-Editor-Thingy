#include "ImgButton.h"

ImgButton::ImgButton(sf::Texture pressedTex, sf::Texture unpressedTex)
{
    this->pressedTex = pressedTex;
    this->unpressedTex = unpressedTex;
    pressed = false;
    button = tgui::Button::create();
    button->getRenderer()->setNormalTexture(this->unpressedTex);
    button->getRenderer()->setBorders(tgui::Borders());
    button->setSize(pressedTex.getSize().x, pressedTex.getSize().y);
}

ImgButton::ImgButton()
{
    pressed = false;
    button = tgui::Button::create();
    button->getRenderer()->setBorders(tgui::Borders());
    button->setSize(32, 32);
}

void ImgButton::swap()
{
    pressed = !pressed;
    if (pressed)
        button->getRenderer()->setNormalTexture(pressedTex);

    else
        button->getRenderer()->setNormalTexture(unpressedTex);
}
