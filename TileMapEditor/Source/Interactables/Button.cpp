#include "Button.h"

Button::Button()
{
}

bool Button::isInside(sf::Vector2i pos) const
{
    return isInside((float)pos.x, (float)pos.y);
}

bool Button::isInside(float x, float y) const
{
    if (x > getPosition().x &&
        y > getPosition().y &&
        x < getPosition().x + getSize().x &&
        y < getPosition().y + getSize().y)
        return true;

    return false;
}
