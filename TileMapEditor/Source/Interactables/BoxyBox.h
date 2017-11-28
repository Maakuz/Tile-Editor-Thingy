#pragma once
#include "SFML\Graphics.hpp"

class BoxyBox : public sf::Drawable
{
public:
    BoxyBox(float x, float y, float width, float height, float borderSize = 5);
    virtual ~BoxyBox() {};

    //void setBorderColor(sf::Color color, sf::Color color2);
    void setBorderColor(sf::Color color);
    //void setColor(sf::Color color, sf::Color color2);
    void setColor(sf::Color color);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::RectangleShape rect;
};