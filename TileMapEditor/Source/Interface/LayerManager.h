#pragma once
#include <vector>
#include "SFML\Graphics.hpp"
#include "Structs.h"
#include <fstream>
#define LAYER_AMOUNT 3

class LayerManager
{
public:
    LayerManager();
    virtual ~LayerManager() {};

    friend std::ostream & operator<<(std::ostream & out, const LayerManager & layerManager);
    friend std::istream & operator>>(std::istream & in, LayerManager & layerManager);

    void update(const std::vector<ActiveTile> & activeTiles, sf::Vector2i mousePos);
    void queueTiles();

    std::vector<ActiveTile> getActiveTilesAt(sf::Vector2i start, sf::Vector2i stop);

    sf::Vector2i getWorkAreaStart() { return workAreaStart; };

    sf::Image getLayerAsImage(int layer) const;

private:
    void processImagePart(sf::Image & image, int layer, int i, int j) const;

    void handleLayerMenu(sf::String clickedItem);

    void differentiateLayes();


    typedef std::vector<std::vector<Tile>> Layer;
    Layer layers[LAYER_AMOUNT];

    int activeLayer;
    sf::Vector2i workAreaStart;

    bool differentiateLayers;
};