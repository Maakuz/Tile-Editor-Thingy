#pragma once
#include "SFML\Graphics.hpp"

class Button : public sf::RectangleShape
{
public:
    Button();
    virtual ~Button() { };

    bool isInside(sf::Vector2i pos) const;
    bool isInside(float x, float y) const;
    bool isActive() const { return active; };

private:
    bool active;
};
