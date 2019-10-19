#include "GUIHandler.h"
#include "Constants.h"
#include <assert.h>
#include "GUI.h"
#include "SFML\Window\Keyboard.hpp"

#define BUTTON_TEXT_SIZE 16
#define DISTANCE 10

GUIHandler::GUIHandler(sf::RenderWindow & window)
{
    gui.setTarget(window);
    Global::gui = &gui;
    active = false;

    //INFO BOX
    {
        tgui::Label::Ptr label = tgui::Label::create();
        label->setText("Active layer: 1");

        tgui::ComboBox::Ptr textureBox = tgui::ComboBox::create();
        textureBox->setPosition(2, 30);

        tgui::Panel::Ptr panel = tgui::Panel::create();
        panel->add(label, Global::Elements::infoBox::layerInfo);
        panel->add(textureBox, Global::Elements::infoBox::textureBox);

        panel->setVisible(false);
        panel->setSize(200, 100);
        panel->setPosition(1000, 50);
        //panel->setBackgroundColor(tgui::Color(sf::Color::White)); dream is dead?

        gui.add(panel, Global::Elements::infoBox::panel);
    }

    //Resize workspace
    {
        tgui::TextBox::Ptr width = tgui::TextBox::create();
        width->setPosition(10, 20);
        width->setSize(200,25);

        tgui::TextBox::Ptr height = tgui::TextBox::create();
        height->setPosition(10, 50);
        height->setSize(200, 25);

        tgui::Button::Ptr confirm = tgui::Button::create();
        confirm->setPosition(220, 20);
        confirm->setTextSize(BUTTON_TEXT_SIZE);
        confirm->setSize(70, 25);
        confirm->setText("Cofirm");

        tgui::Button::Ptr cancel = tgui::Button::create();
        cancel->setPosition(220, 50);
        cancel->setTextSize(BUTTON_TEXT_SIZE);
        cancel->setSize(70, 25);
        cancel->setText("Cancel");


        tgui::Panel::Ptr panel = tgui::Panel::create();  
        panel->setSize(450, 85);
        panel->setPosition(200, TOTAL_BAR_HEIGHT + DISTANCE);

        panel->add(width, Global::Elements::resizeMenu::width);
        panel->add(height, Global::Elements::resizeMenu::height);
        panel->add(confirm, Global::Elements::resizeMenu::confirm);
        panel->add(cancel, Global::Elements::resizeMenu::cancel);
        panel->setVisible(false);

        gui.add(panel, Global::Elements::resizeMenu::panel);
    }

    //Texture importer list
    {
        tgui::ListBox::Ptr textureList = tgui::ListBox::create();
        textureList->setVisible(false);
        textureList->setSize(400, 200);
        textureList->setPosition(400, 20);

        gui.add(textureList, Global::Elements::textureImporter::textureList);
    }
}

void GUIHandler::handleEvents(sf::Event event)
{
   active = gui.handleEvent(event);
}

void GUIHandler::update()
{

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