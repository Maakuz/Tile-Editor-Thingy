#include "Button.h"

Button::Button()
{
    sideShown = TOP_LEFT_BOTTOM;
}

bool Button::isInside(sf::Vector2i pos) const
{
    return isInside((float)pos.x, (float)pos.y);
}

bool Button::isInside(float x, float y) const
{
    if (x >= rect[ENTIRE].getPosition().x &&
        y >= rect[ENTIRE].getPosition().y &&
        x < rect[ENTIRE].getPosition().x + rect[ENTIRE].getSize().x &&
        y < rect[ENTIRE].getPosition().y + rect[ENTIRE].getSize().y)
        return true;

    return false;
}

void Button::setPosition(float x, float y) //TODO: MAKE THESE FUNCTIONS NOT SO STUPID
{
    rect[ENTIRE].setPosition(x, y);
    rect[TOP].setPosition(x, y - rect[TOP].getSize().y);
    rect[RIGHT].setPosition(x + rect[ENTIRE].getSize().x, y);
    rect[BOTTOM].setPosition(x, y + rect[ENTIRE].getSize().y);
    rect[LEFT].setPosition(x - rect[LEFT].getSize().x, y);
}

void Button::setSize(float width, float height)
{
    rect[ENTIRE].setSize({ width, height });
    rect[TOP].setSize({ width, rect[ENTIRE].getOutlineThickness() });
    rect[RIGHT].setSize({ rect[ENTIRE].getOutlineThickness(), height});
    rect[BOTTOM].setSize({ width, rect[ENTIRE].getOutlineThickness() });
    rect[LEFT].setSize({ rect[ENTIRE].getOutlineThickness(), height });
}

void Button::setOutlineThickness(float thickness)
{
    for (int i = 0; i < RECT_AMOUNT; i++)
    {
        rect[i].setOutlineThickness(thickness);
    }
    
}

void Button::setFillColor(sf::Color color)
{
    for (int i = 0; i < RECT_AMOUNT; i++)
    {
        rect[i].setFillColor(color);
    }
}

void Button::setOutlineColor(sf::Color color)
{
    for (int i = 0; i < RECT_AMOUNT; i++)
    {
        rect[i].setOutlineColor(color);
    }
}

void Button::setVisibleSide(SIDES_VISIBLE side)
{
    sideShown = side;
}

Button::operator sf::RectangleShape()
{
    return rect[ENTIRE];
}

Button::operator std::vector<sf::RectangleShape>()
{
    std::vector<sf::RectangleShape> rects;

    switch (sideShown)
    {
    case Button::ENTIRE:
        rects.push_back(rect[ENTIRE]);
        break;

    case Button::TOP:
        rects.push_back(rect[TOP]);
        break;

    case Button::RIGHT:
        rects.push_back(rect[RIGHT]);
        break;

    case Button::BOTTOM:
        rects.push_back(rect[BOTTOM]);
        break;

    case Button::LEFT:
        rects.push_back(rect[LEFT]);
        break;


    case Button::TOP_LEFT:
        rects.push_back(rect[LEFT]);
        rects.push_back(rect[TOP]);
        break;


    case Button::TOP_RIGHT:
        rects.push_back(rect[TOP]);
        rects.push_back(rect[RIGHT]);
        break;


    case Button::BOTTOM_RIGHT:
        rects.push_back(rect[BOTTOM]);
        rects.push_back(rect[RIGHT]);
        break;


    case Button::BOTTOM_LEFT:
        rects.push_back(rect[BOTTOM]);
        rects.push_back(rect[LEFT]);
        break;

    case Button::TOP_LEFT_RIGHT:
        rects.push_back(rect[LEFT]);
        rects.push_back(rect[TOP]);
        rects.push_back(rect[RIGHT]);
        break;


    case Button::RIGHT_LEFT_BOTTOM:
        rects.push_back(rect[BOTTOM]);
        rects.push_back(rect[RIGHT]);
        rects.push_back(rect[LEFT]);
        break;

    case Button::TOP_RIGHT_BOTTOM:
        rects.push_back(rect[TOP]);
        rects.push_back(rect[RIGHT]);
        rects.push_back(rect[BOTTOM]);
        break;

    case Button::TOP_LEFT_BOTTOM:
        rects.push_back(rect[TOP]);
        rects.push_back(rect[BOTTOM]);
        rects.push_back(rect[LEFT]);
        break;

    case Button::TOP_BOTTOM:
        rects.push_back(rect[TOP]);
        rects.push_back(rect[BOTTOM]);
        break;

    case Button::RIGHT_LEFT:
        rects.push_back(rect[LEFT]);
        rects.push_back(rect[RIGHT]);
        break;

    case Button::NONE:
        break;
    default:
        break;
    }

    
    return rects;
}
