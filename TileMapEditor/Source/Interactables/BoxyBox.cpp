#include "BoxyBox.h"

BoxyBox::BoxyBox(float x, float y, float width, float height, float borderSize)
{
    rect.setPosition(x, y);
    rect.setSize({ width, height });
    rect.setOutlineThickness(borderSize);
}


void BoxyBox::setBorderColor(sf::Color color)
{
    rect.setOutlineColor(color);
}

void BoxyBox::setColor(sf::Color color)
{
    rect.setFillColor(color);
}

bool BoxyBox::contains(float x, float y) const
{
    if (x >= rect.getPosition().x &&
        y >= rect.getPosition().y &&
        x < rect.getPosition().x + rect.getSize().x &&
        y < rect.getPosition().y + rect.getSize().y)
        return true;

    return false;
}
