#include "LayerManager.h"
#include "Constants.h"
#include "Queues\TileQueue.h"
#include "Resources.h"
#include "SFML\Window\Mouse.hpp"
#include "GUI.h"
#include <algorithm>

#define DEFAULT_WORK_AREA 100
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
                layers[i][j][k].tileID = 0;
                layers[i][j][k].x = workAreaStart.x + (k * DEFAULT_TILE_SIZE);
                layers[i][j][k].y = workAreaStart.y + (j * DEFAULT_TILE_SIZE);
            }
        }
    }


    Global::gui->get<tgui::MenuBar>(Global::Elements::Menu::bar)->connect("MenuItemClicked", &LayerManager::handleLayerMenu, this);

}

void LayerManager::update(int activeTexture, std::vector<ActiveTile> activeTiles, sf::Vector2i mousePos)
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
                    layers[activeLayer][newY][newX].textureID = activeTexture;
                    layers[activeLayer][newY][newX].tileID = activeTiles[i].id;
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
                if (layers[i][j][k].tileID != 0)
                    TileQueue::get().queue(layers[i][j][k]);
            }
        }
    }
}

Tile LayerManager::getTileAt(sf::Vector2i pos)
{
    return Tile(); //TODO: finish it or remove
}

sf::Image LayerManager::getLayerAsImage(int layer) const
{
    sf::Image image;
    image.create(layers[0][0].size() * DEFAULT_TILE_SIZE, layers[0].size() * DEFAULT_TILE_SIZE, sf::Color::Transparent);

    for (size_t i = 0; i < layers[layer].size(); i++)
    {
        for (size_t j = 0; j < layers[layer][i].size(); j++)
        {
           if (layers[layer][i][j].tileID != 0)
               image.copy(Resources::get().getTexture(layers[layer][i][j].textureID).copyToImage(),
                    layers[layer][i][j].x - workAreaStart.x,
                    layers[layer][i][j].y - workAreaStart.y,
                    Resources::get().getTileRect(layers[layer][i][j].textureID, layers[layer][i][j].tileID),
                    true);
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
    }
    
    if (clickedItem == Global::Elements::Menu::Clickables::layer2)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
        tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

        label->setText("Active layer: 2");
        activeLayer = 1;
    }

    if (clickedItem == Global::Elements::Menu::Clickables::layer3)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
        tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

        label->setText("Active layer: 3");
        activeLayer = 2;
    }
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

    return in;
}
