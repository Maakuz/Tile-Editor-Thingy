#pragma once
#include "SFML\Graphics.hpp"
#include <vector>

class Button
{
public:
    enum SIDES_VISIBLE
    {
        ENTIRE = 0,
        TOP = 1,
        RIGHT = 2,
        BOTTOM = 3,
        LEFT = 4,
        TOP_LEFT = 5,
        TOP_RIGHT = 6,
        BOTTOM_RIGHT = 7,
        BOTTOM_LEFT = 8,
        TOP_LEFT_RIGHT = 9,
        RIGHT_LEFT_BOTTOM = 10,
        TOP_RIGHT_BOTTOM = 11,
        TOP_LEFT_BOTTOM = 12,
        TOP_BOTTOM = 13,
        RIGHT_LEFT = 14,
        NONE = 15
    };

    Button();
    virtual ~Button() { };

    bool isInside(sf::Vector2i pos) const;
    bool isInside(float x, float y) const;
    bool isActive() const { return active; };

    //It is REALLY REALLY recommended to set size before position if both are going to change
    void setPosition(float x, float y);

    //RECOMMEND SETTING OUTLINE BEFORE THIS ONE THIS IS SO STUPID
    void setSize(float width, float height);


    //Can be negative
    void setOutlineThickness(int thickness);

    void setFillColor(sf::Color color);
    void setOutlineColor(sf::Color color);

    void setVisibleSide(SIDES_VISIBLE side);

    operator sf::RectangleShape();
    operator std::vector<sf::RectangleShape>();

private:
    static const int RECT_AMOUNT = 5;

    bool active;
    sf::RectangleShape rect[RECT_AMOUNT];

    SIDES_VISIBLE sideShown;
};
