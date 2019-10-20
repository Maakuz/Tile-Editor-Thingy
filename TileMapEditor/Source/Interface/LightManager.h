#pragma once
#include "Lighting/Light.h"

class LightManager
{
public:
    LightManager();
    ~LightManager();
    LightManager(LightManager&) = delete;

    void update(sf::Vector2i workMousePos, bool guiActive);
    void updateMenu();
    void queueLights();

    void setDefaultLightPos(sf::Vector2f pos) { this->defaultLightPos = pos; };
    bool isPlacingLight() const { return placingLight; };

private:
    std::vector<Light*> lights;
    Light* lightToBePlaced;
    bool placingLight;
    int currentLight;
    sf::Vector2f defaultLightPos;
    float defaultRadius;
    sf::Color defaultColor;
};