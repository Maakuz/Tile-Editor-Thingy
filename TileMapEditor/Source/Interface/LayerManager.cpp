#include "LayerManager.h"
#include "Constants.h"
#include "Queues\TileQueue.h"
#include "TileMaps.h"
#include "SFML\Window\Mouse.hpp"
#include "GUI.h"
#include <algorithm>

LayerManager::LayerManager()
{
    startOver();
}

void LayerManager::update(const std::vector<ActiveTile> & activeTiles, sf::Vector2i mousePos)
{
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (drawing)
        {
            drawing = false;
           
            if (currentState != prevStates.size() - 1)
                prevStates.erase(prevStates.begin() + currentState + 1, prevStates.end());

            if (prevStates.size() >= STATE_AMOUNTS)
            {
                prevStates.erase(prevStates.begin());
                currentState--;
            }

            prevStates.push_back(layers);
            currentState++;

            //printf("State: %d\n", currentState);
            //printf("Size: %d\n", (int)prevStates.size());
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        insertTiles(activeTiles, mousePos);
    }
}

void LayerManager::insertTiles(const std::vector<ActiveTile>& activeTiles, sf::Vector2i mousePos)
{
    if (mousePos.x > workAreaStart.x &&
        mousePos.y > workAreaStart.y) // TODO: check if inside window
    {
        drawing = true;

        //Get what tile the mouse are over
        int x = (mousePos.x - workAreaStart.x) / DEFAULT_TILE_SIZE;
        int y = (mousePos.y - workAreaStart.y) / DEFAULT_TILE_SIZE;

        printf("%d, %d\n", x, y);


        for (size_t i = 0; i < activeTiles.size(); i++)
        {
            int newX = x + activeTiles[i].x;
            int newY = y + activeTiles[i].y;

            if (newX < layers[0][0].size() && newY < layers[0].size())
            {
                layers[activeLayer][newY][newX].textureID = activeTiles[i].textureID;
                layers[activeLayer][newY][newX].tileID = activeTiles[i].tileID;
            }
        }
    }
}

//queue every tile
void LayerManager::queueTiles()
{
    //Rounded up
    int visibleY = std::min(int((WIN_HEIGHT - workAreaStart.y + (DEFAULT_TILE_SIZE * 0.5f)) / DEFAULT_TILE_SIZE), (int)layers[0].size());
    int visibleX = std::min(int((WIN_WIDTH - workAreaStart.x + (DEFAULT_TILE_SIZE * 0.5f)) / DEFAULT_TILE_SIZE), (int)layers[0][0].size());
    
    for (size_t i = 0; i < LAYER_AMOUNT; i++)
    {
        for (size_t j = 0; j < visibleY; j++)
        {
            for (size_t k = 0; k < visibleX; k++)
            {
                if (layers[i][j][k].tileID != -1)
                    TileQueue::get().queue(layers[i][j][k]);
            }
        }
    }
}

std::vector<ActiveTile> LayerManager::getActiveTilesAt(sf::Vector2i start, sf::Vector2i stop)
{
    std::vector<Tile> tiles;

    start -= workAreaStart;
    stop -= workAreaStart;

    start /= DEFAULT_TILE_SIZE;
    stop  /= DEFAULT_TILE_SIZE;

    int xCondition = std::min(stop.x, (int)layers[activeLayer][0].size() - 1);
    int yCondition = std::min(stop.y, (int)layers[activeLayer].size() - 1);

    for (size_t i = start.y; i <= yCondition; i++)
    {
        for (size_t j = start.x; j <= xCondition; j++)
        {
            //if (layers[activeLayer][i][j].tileID != -1) possible but confusing
            tiles.push_back(layers[activeLayer][i][j]);
        }
    }

    std::vector<ActiveTile> newActive;

    for (size_t i = 0; i < tiles.size(); i++)
    {
        ActiveTile active;
        active.tileID = tiles[i].tileID;
        active.textureID = tiles[i].textureID;
        active.x = (tiles[i].x - workAreaStart.x) / DEFAULT_TILE_SIZE - start.x;
        active.y = (tiles[i].y - workAreaStart.y) / DEFAULT_TILE_SIZE - start.y;

        active.box.setSize(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
        active.box.setPosition(tiles[i].x, tiles[i].y);

        newActive.push_back(active);
    }

    return newActive;
}

sf::Image LayerManager::getLayerAsImage(int layer) const
{
    sf::Image image;
    int x = (int)layers[0][0].size();
    int y = (int)layers[0].size();
    image.create(x * DEFAULT_TILE_SIZE, y * DEFAULT_TILE_SIZE, sf::Color::Transparent);


    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            this->processImagePart(image, layer, i, j);
        }
    }

    return image;
}

void LayerManager::setActiveLayer(int layer)
{
    activeLayer = layer;

    differentiateLayers();
}

void LayerManager::toggleHighlightLayers()
{
    hightlightLayers = !hightlightLayers;

    if (!hightlightLayers)
        for (int i = 0; i < LAYER_AMOUNT; i++)
            for (size_t j = 0; j < layers[i].size(); j++)
                for (size_t k = 0; k < layers[i][j].size(); k++)
                    layers[i][j][k].color = sf::Color::White;

    else
        differentiateLayers();
}

void LayerManager::startOver(int width, int height)
{
    activeLayer = 0;
    drawing = false;
    hightlightLayers = true;

    currentState = 0;
    prevStates.clear();

    workAreaStart = sf::Vector2i(TILEMENU_WIDTH, TOTAL_BAR_HEIGHT);

    layers.resize(LAYER_AMOUNT);

    for (int i = 0; i < LAYER_AMOUNT; i++)
    {
        layers[i].resize(height);

        for (int j = 0; j < height; j++)
        {
            layers[i][j].resize(width);

            for (int k = 0; k < width; k++)
            {
                layers[i][j][k].textureID = 0;
                layers[i][j][k].tileID = -1;
                layers[i][j][k].x = workAreaStart.x + (k * DEFAULT_TILE_SIZE);
                layers[i][j][k].y = workAreaStart.y + (j * DEFAULT_TILE_SIZE);
            }
        }
    }

    prevStates.push_back(layers);
}

void LayerManager::resize(int width, int height)
{
    int oldHeight = layers[0].size();
    int oldWidth = layers[0][0].size();

    //slow but easy on me brain
    auto temp = layers;

    for (int i = 0; i < LAYER_AMOUNT; i++)
    {
        layers[i].resize(height);

        for (int j = 0; j < height; j++)
            layers[i][j].resize(width);
    }

    if (width > oldWidth || height > oldHeight)
    {
        for (int i = 0; i < LAYER_AMOUNT; i++)
        {
            for (int j = 0; j < height; j++)
            {
                for (int k = 0; k < width; k++)
                {
                    layers[i][j][k].textureID = 0;
                    layers[i][j][k].tileID = -1;
                    layers[i][j][k].x = workAreaStart.x + (k * DEFAULT_TILE_SIZE);
                    layers[i][j][k].y = workAreaStart.y + (j * DEFAULT_TILE_SIZE);
                }
            }
        }

        for (int i = 0; i < LAYER_AMOUNT; i++)
        {
            for (int j = 0; j < oldHeight; j++)
            {
                for (int k = 0; k < oldWidth; k++)
                {
                    layers[i][j][k] = temp[i][j][k];
                }
            }
        }
    }
    printf("resizeo");
}

void LayerManager::undo(int steps)
{
    currentState -= steps;

    if (currentState < 0)
        currentState = 0;

    if (!prevStates.empty())
        layers = prevStates[currentState];

    differentiateLayers();
}

void LayerManager::redo(int steps)
{
    if (!prevStates.empty())
    {
        currentState += steps;
        if (currentState > prevStates.size() - 1)
            currentState = (int)prevStates.size() - 1;

        else
            layers = prevStates[currentState];
    }

    differentiateLayers();
}

void LayerManager::differentiateLayers()
{
    if (hightlightLayers)
    {
        for (int i = 0; i < LAYER_AMOUNT; i++)
        {
            for (size_t j = 0; j < layers[i].size(); j++)
            {
                for (size_t k = 0; k < layers[i][j].size(); k++)
                {
                    int color = int(((i + 1.f) / (activeLayer + 1.f)) * 255);
                    int transparency = int((float(LAYER_AMOUNT - i) / (LAYER_AMOUNT - activeLayer)) * 255);
                    transparency = std::min(255, transparency);
                    
                    layers[i][j][k].color = sf::Color(color, color, color, transparency);
                }
            }
        }
    }
}

void LayerManager::processImagePart(sf::Image & image, int layer, int i, int j) const
{
    if (layers[layer][i][j].tileID != -1)
        image.copy(TileMaps::get().getTexture(layers[layer][i][j].textureID).copyToImage(),
            layers[layer][i][j].x - workAreaStart.x,
            layers[layer][i][j].y - workAreaStart.y,
            TileMaps::get().getTileRect(layers[layer][i][j].textureID, layers[layer][i][j].tileID),
            true);
}

std::ostream & operator<<(std::ostream & out, const LayerManager & layerManager)
{
    out << layerManager.activeLayer << "\n";
    out << layerManager.layers[0].size() << "\n";
    out << layerManager.layers[0][0].size() << "\n";

    for (size_t i = 0; i < LAYER_AMOUNT; i++)
    {
        for (size_t j = 0; j < layerManager.layers[i].size(); j++)
        {
            for (size_t k = 0; k < layerManager.layers[i][j].size(); k++)
            {
                //total cancer probably
                out << layerManager.layers[i][j][k].textureID << " ";
                out << layerManager.layers[i][j][k].tileID << " ";
            }
        }
    }

    out << "\n";


    return out;
}

std::istream & operator>>(std::istream & in, LayerManager & layerManager)
{
    layerManager.prevStates.clear();
    layerManager.currentState = 0;

    in >> layerManager.activeLayer;
    int x = 0;
    int y = 0;
    in >> y >> x;
    for (int i = 0; i < LAYER_AMOUNT; i++)
    {
        layerManager.layers[i].resize(y);
        for (int j = 0; j < y; j++)
        {
            layerManager.layers[i][j].resize(x);
            for (int k = 0; k < x; k++)
            {
                in >> layerManager.layers[i][j][k].textureID;
                in >> layerManager.layers[i][j][k].tileID;
                layerManager.layers[i][j][k].x = layerManager.workAreaStart.x + (k * DEFAULT_TILE_SIZE);
                layerManager.layers[i][j][k].y = layerManager.workAreaStart.y + (j * DEFAULT_TILE_SIZE);
            }
        }
    }

    layerManager.prevStates.push_back(layerManager.layers);


    tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
    tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

    label->setText("Active layer: " + std::to_string(layerManager.activeLayer + 1));

    layerManager.differentiateLayers();

    return in;
}
