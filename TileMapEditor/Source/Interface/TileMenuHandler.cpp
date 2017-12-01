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
            if (!tileBox.contains(event.mouseButton.x, event.mouseButton.y))
            {
                pressedPos = { event.mouseButton.x, event.mouseButton.y };

                rightClicking = true;
            }
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
            handleLayerSelection(pressedPos, mousePos);
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
        std::vector<sf::RectangleShape> rects = i.box;

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
            buttons[j + TILEMENU_X_AREA * i].setSize(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
            buttons[j + TILEMENU_X_AREA * i].setPosition(float(j * DEFAULT_TILE_SIZE + offset.x), float(i * DEFAULT_TILE_SIZE + offset.y));
        }
    }
}

void TileMenuHandler::handleFileMenu(sf::String button)
{
    if (button == Global::Elements::Menu::Clickables::saveFile)
        fileManager.save(layerManager);

    if (button == Global::Elements::Menu::Clickables::openFile)
    {
        fileManager.load(layerManager);
        activeTiles.clear();
    }

    if (button == Global::Elements::Menu::Clickables::exportLayers)
        fileManager.exportTextures(layerManager);
}

void TileMenuHandler::handleBlockSelection(sf::Vector2i start, sf::Vector2i stop)
{
    bool cleared = false;

    start /= DEFAULT_TILE_SIZE;
    stop /= DEFAULT_TILE_SIZE;

    swapStartAndStopPosition(start, stop);

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
                        activeTiles.clear();

                        cleared = true;
                    }

                    ActiveTile activeTile;
                    activeTile.id = (int)i;
                    activeTile.x = j - start.x;
                    activeTile.y = k - start.y;
                    activeTile.box.setSize(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
                    activeTile.box.setPosition(j * DEFAULT_TILE_SIZE + offset.x, k * DEFAULT_TILE_SIZE + offset.y);
                    activeTiles.push_back(activeTile);

                }
            }
        }
    }

    createActiveBounds(activeTiles);
}

void TileMenuHandler::handleLayerSelection(sf::Vector2i start, sf::Vector2i stop)
{
    swapStartAndStopPosition(start, stop);

    this->activeTiles = layerManager.getActiveTilesAt(start, stop);

    createActiveBounds(activeTiles);

}

void TileMenuHandler::createActiveBounds(std::vector<ActiveTile>& tiles) // TODO: Make this faster //
{
    //To figure out if this is a corner or nay
    for (ActiveTile & i : tiles)
    {
        bool leftNeighborExist = false;
        bool rightNeighborExist = false;
        bool topNeighborExist = false;
        bool bottomNeighborExist = false;

        for (const ActiveTile & neighbor : tiles)
        {
            if (abs(i.x - neighbor.x) <= 1 && abs(i.y - neighbor.y) <= 1)
            {
                if (i.x < neighbor.x)
                    rightNeighborExist = true;

                else if (i.x > neighbor.x)
                    leftNeighborExist = true;

                else if (i.y < neighbor.y)
                    bottomNeighborExist = true;

                else if (i.y > neighbor.y)
                    topNeighborExist = true;
            }
        }

        if (leftNeighborExist && rightNeighborExist && topNeighborExist && bottomNeighborExist)
            i.box.setVisibleSide(Button::SIDES::NONE);



        else if (leftNeighborExist && rightNeighborExist && topNeighborExist)
            i.box.setVisibleSide(Button::SIDES::BOTTOM);

        else if (leftNeighborExist && rightNeighborExist && bottomNeighborExist)
            i.box.setVisibleSide(Button::SIDES::TOP);

        else if (leftNeighborExist && topNeighborExist && bottomNeighborExist)
            i.box.setVisibleSide(Button::SIDES::RIGHT);

        else if (rightNeighborExist && topNeighborExist && bottomNeighborExist)
            i.box.setVisibleSide(Button::SIDES::LEFT);



        else if (rightNeighborExist && leftNeighborExist)
            i.box.setVisibleSide(Button::SIDES::TOP_BOTTOM);

        else if (topNeighborExist && bottomNeighborExist)
            i.box.setVisibleSide(Button::SIDES::RIGHT_LEFT);



        else if (topNeighborExist && rightNeighborExist)
            i.box.setVisibleSide(Button::SIDES::BOTTOM_LEFT);

        else if (topNeighborExist && leftNeighborExist)
            i.box.setVisibleSide(Button::SIDES::BOTTOM_RIGHT);

        else if (bottomNeighborExist && rightNeighborExist)
            i.box.setVisibleSide(Button::SIDES::TOP_LEFT);

        else if (bottomNeighborExist && leftNeighborExist)
            i.box.setVisibleSide(Button::SIDES::TOP_RIGHT);


        else if (bottomNeighborExist)
            i.box.setVisibleSide(Button::SIDES::TOP_LEFT_RIGHT);

        else if (rightNeighborExist)
            i.box.setVisibleSide(Button::SIDES::TOP_LEFT_BOTTOM);

        else if (topNeighborExist)
            i.box.setVisibleSide(Button::SIDES::RIGHT_LEFT_BOTTOM);

        else if (leftNeighborExist)
            i.box.setVisibleSide(Button::SIDES::TOP_RIGHT_BOTTOM);

        else
            i.box.setVisibleSide(Button::SIDES::ENTIRE);
    }
}

void TileMenuHandler::swapStartAndStopPosition(sf::Vector2i & start, sf::Vector2i & stop)
{

    if (start.x > stop.x)
        std::swap(start.x, stop.x);

    if (start.y > stop.y)
        std::swap(start.y, stop.y);

}
