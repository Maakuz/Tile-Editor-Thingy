#pragma once
#include <vector>
#include "SFML\Graphics.hpp"
#include "Structs.h"
#include <fstream>
#define LAYER_AMOUNT 3
#define STATE_AMOUNTS 50
#define DEFAULT_WORK_AREA_X 20
#define DEFAULT_WORK_AREA_Y 20

class LayerManager
{
public:
    LayerManager();
    virtual ~LayerManager() {};

    friend std::ostream & operator<<(std::ostream & out, const LayerManager & layerManager);
    friend std::istream & operator>>(std::istream & in, LayerManager & layerManager);

    void update(const std::vector<ActiveTile> & activeTiles, sf::Vector2i mousePos);
    void insertTiles(const std::vector<ActiveTile> & activeTiles, sf::Vector2i mousePos);
    
    void queueTiles();

    std::vector<ActiveTile> getActiveTilesAt(sf::Vector2i start, sf::Vector2i stop);

    sf::Vector2i getWorkAreaStart() { return workAreaStart; };

    sf::Image getLayerAsImage(int layer) const;

    void setActiveLayer(int layer);

    void toggleHighlightLayers();

    //Clears all layers
    void startOver(int width = DEFAULT_WORK_AREA_X, int height = DEFAULT_WORK_AREA_Y);
    void resize(int width = DEFAULT_WORK_AREA_X, int height = DEFAULT_WORK_AREA_Y);

    void undo(int steps = 1);
    void redo(int steps = 1);

private:
    void processImagePart(sf::Image & image, int layer, int i, int j) const;

    void differentiateLayers();


    typedef std::vector<std::vector<Tile>> Layer;
    std::vector<Layer> layers;
    std::vector<std::vector<Layer>> prevStates;
    int currentState;

    int activeLayer;
    sf::Vector2i workAreaStart;

    bool hightlightLayers;
    bool drawing;
};