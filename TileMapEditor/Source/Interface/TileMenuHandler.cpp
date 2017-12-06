#include "TileMenuHandler.h"
#include "Queues\TileQueue.h"
#include "Queues\OverlayQueue.h"
#include "Queues\BackgroundQueue.h"
#include "Structs.h"
#include "Constants.h"
#include "SFML\Window\Mouse.hpp"
#include "GUI.h"
#include "TileMaps.h"



TileMenuHandler::TileMenuHandler() :
    tileBox(TILEMENU_BORDER_SIZE, TILEMENU_BORDER_SIZE + TOTAL_BAR_HEIGHT, DEFAULT_TILE_SIZE * TILEMENU_X_AREA, DEFAULT_TILE_SIZE * TILEMENU_Y_AREA, TILEMENU_BORDER_SIZE)
{
    tileBox.setColor(sf::Color(0, 100, 200));
    tileBox.setBorderColor(sf::Color(100, 100, 100));
    activeTileTexture = 0;
    offset.x = TILEMENU_BORDER_SIZE;
    offset.y = TILEMENU_BORDER_SIZE + TOTAL_BAR_HEIGHT;
    
    createTileButtons();

    #pragma region Buttons

    Global::gui->get<tgui::MenuBar>(Global::Elements::Menu::bar)->connect("MenuItemClicked", &TileMenuHandler::handleFileMenu, this);
    Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel)->get(Global::Elements::infoBox::textureBox)->connect("itemselected", &TileMenuHandler::setActiveTexture, this);

    Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel)->get(Global::Elements::savebox::saveButton)->connect("clicked", &TileMenuHandler::saveFile, this);
    Global::gui->get<tgui::Panel>(Global::Elements::loadbox::panel)->get(Global::Elements::loadbox::loadButton)->connect("clicked", &TileMenuHandler::loadFile, this);
   
    Global::gui->get(Global::Elements::textureImporter::textureList)->connect("DoubleClicked", &TileMenuHandler::importTexture, this);

    Global::gui->get<tgui::Panel>(Global::Elements::imagemenu::panel)->get(Global::Elements::imagemenu::newButton)->connect("clicked", 
        [&]() 
    {
        if (!anyWindowsOpen())
        {
            layerManager.startOver();
            activeTiles.clear();
        }
    });

    Global::gui->get<tgui::Panel>(Global::Elements::imagemenu::panel)->get(Global::Elements::imagemenu::openButton)->connect("clicked",
        [&]()
    {
        if (!anyWindowsOpen())
            loadWindow.openWindow();
    });

    Global::gui->get<tgui::Panel>(Global::Elements::imagemenu::panel)->get(Global::Elements::imagemenu::saveButton)->connect("clicked",
        [&]()
    {
        if (!anyWindowsOpen())
            saveWindow.openWindow();
    });

    Global::gui->get<tgui::Panel>(Global::Elements::imagemenu::panel)->get(Global::Elements::imagemenu::eraserButton)->connect("clicked", &TileMenuHandler::equipEraser, this);
    #pragma endregion

    selectingBlocks = false;
    rightClicking = false;
    activeTileTexture = -1;
}

void TileMenuHandler::handleEvent(sf::Event event, bool guiBlock)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left && !rightClicking && !guiBlock)
        {
            pressedPos = { event.mouseButton.x, event.mouseButton.y };

            if (tileBox.contains(pressedPos.x, pressedPos.y))
                selectingBlocks = true;
        }
        
        if (event.mouseButton.button == sf::Mouse::Right && !selectingBlocks && !guiBlock)
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

    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::F10)
            handleFileMenu(Global::Elements::Menu::Clickables::infoBox);

        if (event.key.code == sf::Keyboard::Z && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            layerManager.undo();

        if (event.key.code == sf::Keyboard::Y && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            layerManager.redo();

        if (event.key.code == sf::Keyboard::C && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            copiedTiles = activeTiles;

        if (event.key.code == sf::Keyboard::X && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            copiedTiles = activeTiles;

            std::vector<ActiveTile> eraseTiles = activeTiles;

            for (size_t i = 0; i < eraseTiles.size(); i++)
            {
                eraseTiles[i].tileID = -1;
            }

            layerManager.insertTiles(eraseTiles, pressedPos);
            activeTiles.clear();
        }

        if (event.key.code == sf::Keyboard::V && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            layerManager.insertTiles(copiedTiles, pressedPos);

        if (event.key.code == sf::Keyboard::E)
            equipEraser();
        break;
    }
}

void TileMenuHandler::update(sf::Vector2i mousePos)
{
    if (saveWindow.isOpen() || loadWindow.isOpen())
        return;

    if (!selectingBlocks)
    {
        if (rightClicking)
        {
            handleLayerSelection(pressedPos, mousePos);
        }

        else if (activeTileTexture != -1)
            layerManager.update(activeTiles, mousePos);
    }

    else if (activeTileTexture != -1)
    {
        handleBlockSelection(pressedPos - offset, mousePos - offset);
    }
}

void TileMenuHandler::queueItems()
{
    layerManager.queueTiles();

    if (activeTileTexture != -1)
    {
        sf::Vector2i bounds = TileMaps::get().getSheetSize(activeTileTexture);

        for (int i = 0; i < TILEMENU_Y_AREA; i++)
        {
            for (int j = 0; j < TILEMENU_X_AREA; j++)
            {
                if (j + TILEMENU_X_AREA * i < bounds.x * bounds.y)
                {
                    Tile activeTile;
                    activeTile.textureID = activeTileTexture;
                    activeTile.tileID = j + TILEMENU_X_AREA * i;
                    activeTile.x = j * DEFAULT_TILE_SIZE + offset.x;
                    activeTile.y = i * DEFAULT_TILE_SIZE + offset.y;

                    TileQueue::get().queue(activeTile);
                }
            }
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
    if (anyWindowsOpen())
        return;

    if (button == Global::Elements::Menu::Clickables::newFile)
    {
        layerManager.startOver();
        activeTiles.clear();
    }

    if (button == Global::Elements::Menu::Clickables::saveFile)
        saveWindow.openWindow();

    if (button == Global::Elements::Menu::Clickables::openFile)
        loadWindow.openWindow();

    if (button == Global::Elements::Menu::Clickables::exportLayers)
        fileManager.exportTextures(layerManager);

    if (button == Global::Elements::Menu::Clickables::importTexture)
        fileManager.importTexure();

    if (button == Global::Elements::Menu::Clickables::undo)
        layerManager.undo();

    if (button == Global::Elements::Menu::Clickables::redo)
        layerManager.redo();

    if (button == Global::Elements::Menu::Clickables::layer1)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
        tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

        label->setText("Active layer: 1");
        layerManager.setActiveLayer(0);
    }

    if (button == Global::Elements::Menu::Clickables::layer2)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
        tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

        label->setText("Active layer: 2");
        layerManager.setActiveLayer(1);
    }

    if (button == Global::Elements::Menu::Clickables::layer3)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);
        tgui::Label::Ptr label = panel->get<tgui::Label>(Global::Elements::infoBox::layerInfo);

        label->setText("Active layer: 3");
        layerManager.setActiveLayer(2);
    }

    if (button == Global::Elements::Menu::Clickables::darken)
        layerManager.toggleHighlightLayers();

    if (button == Global::Elements::Menu::Clickables::infoBox)
    {
        tgui::Panel::Ptr panel = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel);

        if (panel->isVisible())
            panel->hide();

        else
            panel->show();
    }

}

void TileMenuHandler::handleBlockSelection(sf::Vector2i start, sf::Vector2i stop)
{
    bool cleared = false;

    start /= DEFAULT_TILE_SIZE;
    stop /= DEFAULT_TILE_SIZE;

    swapStartAndStopPosition(start, stop);

    sf::Vector2i bounds = TileMaps::get().getSheetSize(activeTileTexture);

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

                    if (i < bounds.x * bounds.y)
                    {
                        ActiveTile activeTile;
                        activeTile.tileID = (int)i;
                        activeTile.textureID = activeTileTexture;
                        activeTile.x = j - start.x;
                        activeTile.y = k - start.y;
                        activeTile.box.setSize(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
                        activeTile.box.setPosition(j * DEFAULT_TILE_SIZE + offset.x, k * DEFAULT_TILE_SIZE + offset.y);
                        activeTiles.push_back(activeTile);
                    }

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
            if (abs(i.x - neighbor.x) + abs(i.y - neighbor.y) <= 1)
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

void TileMenuHandler::setActiveTexture(sf::String name, sf::String path)
{
    activeTileTexture = TileMaps::get().getTextureIndex(name);
}

void TileMenuHandler::importTexture(sf::String name, sf::String path)
{
    activeTileTexture = fileManager.addTexture(name, path);
}

void TileMenuHandler::equipEraser()
{
    activeTiles.clear();
    ActiveTile eraser;
    eraser.tileID = -1;
    eraser.x = 0;
    eraser.y = 0;

    activeTiles.push_back(eraser);
    erasing = true;
}

bool TileMenuHandler::anyWindowsOpen()
{
    if (loadWindow.isOpen())
        return true;

    if (saveWindow.isOpen())
        return true;

    return false;
}

void TileMenuHandler::saveFile()
{
    auto panel = Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel);
    std::string name = panel->get<tgui::TextBox>(Global::Elements::savebox::fileName)->getText();

    fs::path dir = saveWindow.getPath();

    fileManager.save(layerManager, dir / name);

    saveWindow.closeWindow();
}

void TileMenuHandler::loadFile()
{
    auto panel = Global::gui->get<tgui::Panel>(Global::Elements::loadbox::panel);
    std::string name = panel->get<tgui::ListBox>(Global::Elements::loadbox::paths)->getSelectedItem();

    fs::path dir = loadWindow.getPath();

    fileManager.load(layerManager, dir / name);

    activeTiles.clear();

    loadWindow.closeWindow();
}
