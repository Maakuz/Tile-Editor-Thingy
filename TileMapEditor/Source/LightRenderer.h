#pragma once
#include "ShaderHandler.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class LightRenderer
{
public:
    LightRenderer();
    ~LightRenderer() {};

    void renderLights(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

private:
    ShaderHandler shaders;
    sf::RenderTexture renderTargets[3];
    sf::RectangleShape fullscreenboi;
};