#include "TileMenuHandler.h"
#include "Queues\TileQueue.h"
#include "Queues\OverlayQueue.h"
#include "Queues\BackgroundQueue.h"
#include "Structs.h"
#include "Constants.h"
#include "SFML\Window\Mouse.hpp"
#include "GUI.h"


TileMenuHandler::TileMenuHandler() :
    tileBox(TILEMENU_BORDER_SIZE, TILEMENU_BORDER_SIZE + MENU_BAR_HEIGHT, DEFAULT_TILE_SIZE * TILEMENU_X_AREA, DEFAULT_TILE_SIZE * TILEMENU_Y_AREA, TILEMENU_BORDER_SIZE)
{
    tileBox.setColor(sf::Color(0, 100, 200));
    tileBox.setBorderColor(sf::Color(100, 100, 100));
    activeTileTexture = 0;
    offset.x = TILEMENU_BORDER_SIZE;
    offset.y = TILEMENU_BORDER_SIZE + MENU_BAR_HEIGHT;
    
    createTileButtons();

    Global::gui->get<tgui::MenuBar>(Global::Elements::Menu::bar)->connect("MenuItemClicked", &TileMenuHandler::handleFileMenu, this);
    selectingBlocks = false;
    rightClicking = false;
}

void TileMenuHandler::handleEvent(sf::Event event)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left && !rightClicking)
        {
            pressedPos = { event.mouseButton.x, event.mouseButton.y };

            if (tileBox.contains(pressedPos.x, pressedPos.y))
                selectingBlocks = true;
        }
        
        if (event.mouseButton.button == sf::Mouse::Right && !selectingBlocks)
        {
            pressedPos = { event.mouseButton.x, event.mouseButton.y };

            rightClicking = true;
        }
        break;


    case sf::Event::MouseButtonReleased:

        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (selectingBlocks)
            {
                selectingBlocks = false;
            }
        }

        if (event.mouseButton.button == sf::Mouse::Right)
        {
            if (rightClicking)
                rightClicking = false;
        }
        break;
    }
}

void TileMenuHandler::update(sf::Vector2i mousePos)
{
    if (!selectingBlocks)
    {
        if (rightClicking)
        {
            handleLayerSelection(pressedPos - offset, mousePos - offset);
        }

        else
            layerManager.update(activeTileTexture, activeTiles, mousePos);
    }

    else
    {
        handleBlockSelection(pressedPos - offset, mousePos - offset);
    }
}

void TileMenuHandler::queueItems()
{
    layerManager.queueTiles();


    for (int i = 0; i < TILEMENU_Y_AREA; i++)
    {
        for (int j = 0; j < TILEMENU_X_AREA; j++)
        {
            Tile activeTile;
            activeTile.textureID = activeTileTexture;
            activeTile.tileID = j + TILEMENU_X_AREA * i;
            activeTile.x = j * DEFAULT_TILE_SIZE + offset.x;
            activeTile.y = i * DEFAULT_TILE_SIZE + offset.y;

            TileQueue::get().queue(activeTile);
        }
    }

    BackgroundQueue::get().queue(tileBox.getRect());

    for each (ActiveTile i in activeTiles)
    {
        std::vector<sf::RectangleShape> rects = buttons[i.id];

        for each (sf::RectangleShape rect in rects)
        {
            OverlayQueue::get().queue(rect);
        }
    }
}

void TileMenuHandler::createTileButtons()
{
    buttons.resize(TILEMENU_Y_AREA * TILEMENU_X_AREA);

    for (int i = 0; i < TILEMENU_Y_AREA; i++)
    {
        for (int j = 0; j < TILEMENU_X_AREA; j++)
        {
            buttons[j + TILEMENU_X_AREA * i].setOutlineThickness(-2);
            buttons[j + TILEMENU_X_AREA * i].setSize(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
            buttons[j + TILEMENU_X_AREA * i].setPosition(float(j * DEFAULT_TILE_SIZE + offset.x), float(i * DEFAULT_TILE_SIZE + offset.y));
            buttons[j + TILEMENU_X_AREA * i].setFillColor(sf::Color::Transparent);
            buttons[j + TILEMENU_X_AREA * i].setOutlineColor(sf::Color::White);
        }
    }
}

void TileMenuHandler::handleFileMenu(sf::String button)
{
    if (button == Global::Elements::Menu::Clickables::saveFile)
        fileManager.save(layerManager);

    if (button == Global::Elements::Menu::Clickables::openFile)
        fileManager.load(layerManager);

    if (button == Global::Elements::Menu::Clickables::exportLayers)
        fileManager.exportTextures(layerManager);
}

void TileMenuHandler::handleBlockSelection(sf::Vector2i start, sf::Vector2i stop)
{
    bool cleared = false;

    readyStartAndStopPosition(start, stop);

    for (int k = start.y; k <= stop.y; k++)
    {
        for (int j = start.x; j <= stop.x; j++)
        {
            for (size_t i = 0; i < buttons.size(); i++)
            {
                if (buttons[i].isInside(float(j * DEFAULT_TILE_SIZE + offset.x), float(k * DEFAULT_TILE_SIZE + offset.y)))
                {
                    if (!cleared)
                    {
                        for (ActiveTile i : activeTiles)
                        {
                            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::NONE);
                        }
                        activeTiles.clear();

                        cleared = true;
                    }

                    ActiveTile activeTile;
                    activeTile.id = (int)i;
                    activeTile.x = j - start.x;
                    activeTile.y = k - start.y;

                    activeTiles.push_back(activeTile);

                }
            }
        }
    }

    createActiveBounds(activeTiles);
}

void TileMenuHandler::handleLayerSelection(sf::Vector2i start, sf::Vector2i stop)
{
    readyStartAndStopPosition(start, stop);

    std::vector<Tile> newSelectedTiles = layerManager.getTilesAt(start, stop);
}

void TileMenuHandler::createActiveBounds(std::vector<ActiveTile> tiles)
{
    //To figure out if this is a corner or nay
    for (ActiveTile i : tiles)
    {
        bool leftNeighborExist = false;
        bool rightNeighborExist = false;
        bool topNeighborExist = false;
        bool bottomNeighborExist = false;

        for (ActiveTile neighbor : tiles)
        {
            if (i.x < neighbor.x)
                rightNeighborExist = true;

            if (i.x > neighbor.x)
                leftNeighborExist = true;

            if (i.y < neighbor.y)
                bottomNeighborExist = true;

            if (i.y > neighbor.y)
                topNeighborExist = true;
        }

        if (leftNeighborExist && rightNeighborExist && topNeighborExist && bottomNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::NONE);



        else if (leftNeighborExist && rightNeighborExist && topNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::BOTTOM);

        else if (leftNeighborExist && rightNeighborExist && bottomNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::TOP);

        else if (leftNeighborExist && topNeighborExist && bottomNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::RIGHT);

        else if (rightNeighborExist && topNeighborExist && bottomNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::LEFT);



        else if (rightNeighborExist && leftNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::TOP_BOTTOM);

        else if (topNeighborExist && bottomNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::RIGHT_LEFT);



        else if (topNeighborExist && rightNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::BOTTOM_LEFT);

        else if (topNeighborExist && leftNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::BOTTOM_RIGHT);

        else if (bottomNeighborExist && rightNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::TOP_LEFT);

        else if (bottomNeighborExist && leftNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::TOP_RIGHT);


        else if (bottomNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::TOP_LEFT_RIGHT);

        else if (rightNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::TOP_LEFT_BOTTOM);

        else if (topNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::RIGHT_LEFT_BOTTOM);

        else if (leftNeighborExist)
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::TOP_RIGHT_BOTTOM);

        else
            buttons[i.id].setVisibleSide(Button::SIDES_VISIBLE::ENTIRE);
    }
}

void TileMenuHandler::readyStartAndStopPosition(sf::Vector2i & start, sf::Vector2i & stop)
{
    start /= DEFAULT_TILE_SIZE;
    stop /= DEFAULT_TILE_SIZE;


    if (start.x > stop.x)
        std::swap(start.x, stop.x);

    if (start.y > stop.y)
        std::swap(start.y, stop.y);

}
