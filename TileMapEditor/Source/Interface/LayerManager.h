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

    void update(int activeTexture, std::vector<ActiveTile> activeTiles, sf::Vector2i mousePos);
    void queueTiles();

    Tile getTileAt(sf::Vector2i pos);

    sf::Image getLayerAsImage(int layer) const;

private:
    void handleLayerMenu(sf::String clickedItem);

    typedef std::vector<std::vector<Tile>> Layer;
    Layer layers[LAYER_AMOUNT];

    int activeLayer;
    sf::Vector2i workAreaStart;
};