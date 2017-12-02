#include "GUIHandler.h"
#include "Constants.h"
#include <assert.h>
#include "GUI.h"
#include "SFML\Window\Keyboard.hpp"

#define TEST(button) 

GUIHandler::GUIHandler(sf::RenderWindow & window)
{
    gui.setWindow(window);
    Global::gui = &gui;
    active = false;

    tgui::MenuBar::Ptr bar = tgui::MenuBar::create();

    bar->setPosition(0, 0);
    bar->setSize(1280, MENU_BAR_HEIGHT);

    bar->addMenu("File");
    bar->addMenuItem("File", Global::Elements::Menu::Clickables::newFile);
    bar->addMenuItem("File", Global::Elements::Menu::Clickables::openFile);
    bar->addMenuItem("File", Global::Elements::Menu::Clickables::saveFile);
    bar->addMenuItem("File", Global::Elements::Menu::Clickables::exitFile);

    bar->addMenu("Edit");
    bar->addMenuItem("Edit", Global::Elements::Menu::Clickables::undo);
    bar->addMenuItem("Edit", Global::Elements::Menu::Clickables::importTexture);

    bar->addMenu("Layer");
    bar->addMenuItem("Layer", Global::Elements::Menu::Clickables::layer1);
    bar->addMenuItem("Layer", Global::Elements::Menu::Clickables::layer2);
    bar->addMenuItem("Layer", Global::Elements::Menu::Clickables::layer3);

    bar->addMenu("View");
    bar->addMenuItem("View", Global::Elements::Menu::Clickables::infoBox);
    bar->addMenuItem("View", Global::Elements::Menu::Clickables::darken);

    bar->addMenu("Tools");
    bar->addMenuItem("Tools", Global::Elements::Menu::Clickables::exportLayers);

    bar->connect("MenuItemClicked", &GUIHandler::handleMenuBarClick, this);

    gui.add(bar, Global::Elements::Menu::bar);

   

    tgui::Label::Ptr label = tgui::Label::create();
    label->setText("Active layer: 1");

    tgui::ComboBox::Ptr textureBox = tgui::ComboBox::create();
    textureBox->setPosition(2, 30);

    tgui::Panel::Ptr panel = tgui::Panel::create();
    panel->add(label, Global::Elements::infoBox::layerInfo);
    panel->add(textureBox, Global::Elements::infoBox::textureBox);
    
    panel->hide();
    panel->setSize(200, 100);
    panel->setPosition(1000, 50);
    panel->setBackgroundColor(tgui::Color(sf::Color::White));

    gui.add(panel, Global::Elements::infoBox::panel);


    tgui::ListBox::Ptr textureList = tgui::ListBox::create();
    textureList->hide();
    textureList->setSize(400, 200);
    textureList->setPosition(400, 20);

    gui.add(textureList, Global::Elements::textureImporter::textureList);


}

void GUIHandler::handleEvents(sf::Event event)
{
   active = gui.handleEvent(event);
}

void GUIHandler::update()
{
    static bool wasF10 = false;
    static bool isF10 = false;
    wasF10 = isF10;
    isF10 = sf::Keyboard::isKeyPressed(sf::Keyboard::F10);
    if (!wasF10 && isF10)
        handleMenuBarClick(Global::Elements::Menu::Clickables::infoBox);
}

void GUIHandler::drawGui()
{
    gui.draw();
}

void GUIHandler::crashEverything()
{
    printf("pain!");
    crashEverything();
}

void GUIHandler::handleMenuBarClick(sf::String itemClicked)
{
    if (itemClicked == Global::Elements::Menu::Clickables::infoBox)
    {
        tgui::Panel::Ptr panel = gui.get<tgui::Panel>(Global::Elements::infoBox::panel);

        if (panel->isVisible())
            panel->hide();

        else
            panel->show();
    }

}
