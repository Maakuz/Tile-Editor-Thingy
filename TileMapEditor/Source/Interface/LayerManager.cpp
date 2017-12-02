#include "LayerManager.h"
#include "Constants.h"
#include "Queues\TileQueue.h"
#include "TileMaps.h"
#include "SFML\Window\Mouse.hpp"
#include "GUI.h"
#include <algorithm>

#define DEFAULT_WORK_AREA 10
LayerManager::LayerManager()
{
    activeLayer = 0;
    workAreaStart = sf::Vector2i(TILEMENU_WIDTH, MENU_BAR_HEIGHT);

    for (int i = 0; i < LAYER_AMOUNT; i++)
    {
        layers[i].resize(DEFAULT_WORK_AREA);

        for (int j = 0; j < DEFAULT_WORK_AREA; j++)
        {
            layers[i][j].resize(DEFAULT_WORK_AREA);

            for (int k = 0; k < DEFAULT_WORK_AREA; k++)
            {
                layers[i][j][k].textureID = 0;
                layers[i][j][k].tileID = -1;
                layers[i][j][k].x = workAreaStart.x + (k * DEFAULT_TILE_SIZE);
                layers[i][j][k].y = workAreaStart.y + (j * DEFAULT_TILE_SIZE);
            }
        }
    }


    Global::gui->get<tgui::MenuBar>(Global::Elements::Menu::bar)->connect("MenuItemClicked", &LayerManager::handleLayerMenu, this);

}

void LayerManager::update(const std::vector<ActiveTile> & activeTiles, sf::Vector2i mousePos)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (mousePos.x > workAreaStart.x &&
            mousePos.y > workAreaStart.y) // TODO: check if inside window
        {
            //Get what tile the mouse are over
            int x = (mousePos.x - workAreaStart.x) / DEFAULT_TILE_SIZE;
            int y = (mousePos.y - workAreaStart.y) / DEFAULT_TILE_SIZE;

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


    for (size_t i = 0; i < y; i++)
    {
        for (size_t j = 0; j < x; j++)
        {
            this->processImagePart(image, layer, i, j);
        }
    }

    return image;
}

void LayerManager::handleLayerMenu(sf::String clickedItem)
{
    if (clickedItem == Global::Elements::Menu::Clickables::layer1)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
        tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

        label->setText("Active layer: 1");
        activeLayer = 0;

        differentiateLayes();
    }
    
    if (clickedItem == Global::Elements::Menu::Clickables::layer2)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
        tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

        label->setText("Active layer: 2");
        activeLayer = 1;

        differentiateLayes();
    }

    if (clickedItem == Global::Elements::Menu::Clickables::layer3)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
        tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

        label->setText("Active layer: 3");
        activeLayer = 2;

        differentiateLayes();
    }

    if (clickedItem == Global::Elements::Menu::Clickables::darken)
    {
        differentiateLayers = !differentiateLayers;

        if (!differentiateLayers)
            for (int i = 0; i < LAYER_AMOUNT; i++)
                for (size_t j = 0; j < layers[i].size(); j++)
                    for (size_t k = 0; k < layers[i][j].size(); k++)
                        layers[i][j][k].color = sf::Color::White;

        else
            differentiateLayes();
    }
}

void LayerManager::differentiateLayes()
{
    if (differentiateLayers)
    {
        for (int i = 0; i < LAYER_AMOUNT; i++)
        {
            for (size_t j = 0; j < layers[i].size(); j++)
            {
                for (size_t k = 0; k < layers[i][j].size(); k++)
                {
                    int color = ((i + 1.f) / (activeLayer + 1.f)) * 255;
                    int transparency = (float(LAYER_AMOUNT - i) / (LAYER_AMOUNT - activeLayer)) * 255;
                    transparency = std::min(255, transparency);
                    
                    layers[i][j][k].color = sf::Color(color, color, color, transparency);
                }
            }
        }
    }
}

void LayerManager::processImagePart(sf::Image & image, int layer, int i, int j) const
{
    if (layers[layer][i][j].tileID != 0)
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
                out << layerManager.layers[i][j][k].x << " ";
                out << layerManager.layers[i][j][k].y<< " ";
            }
        }
    }

    out << "\n";


    return out;
}

std::istream & operator>>(std::istream & in, LayerManager & layerManager)
{
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
                in >> layerManager.layers[i][j][k].x;
                in >> layerManager.layers[i][j][k].y;
            }
        }
    }

    tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
    tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

    label->setText("Active layer: " + std::to_string(layerManager.activeLayer + 1));

    layerManager.differentiateLayes();

    return in;
}
