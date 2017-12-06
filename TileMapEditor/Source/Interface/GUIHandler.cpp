#include "GUIHandler.h"
#include "Constants.h"
#include <assert.h>
#include "GUI.h"
#include "SFML\Window\Keyboard.hpp"

#define BUTTON_TEXT_SIZE 16
#define DISTANCE 10

GUIHandler::GUIHandler(sf::RenderWindow & window)
{
    gui.setWindow(window);
    Global::gui = &gui;
    active = false;

    //IMAGE PANEL
    {
        tgui::Panel::Ptr panel = tgui::Panel::create();
        panel->setSize(WIN_WIDTH, IMAGE_BAR_HEIGHT);
        panel->setPosition(0, MENU_BAR_HEIGHT);

        const int offset = 4;
        const int buttonCount = 5;
        sf::Texture clickTexture[buttonCount];
        sf::Texture texture[buttonCount];
        sf::String names[buttonCount];

        names[0] = Global::Elements::imagemenu::newButton;
        names[1] = Global::Elements::imagemenu::openButton;
        names[2] = Global::Elements::imagemenu::saveButton;
        names[3] = Global::Elements::imagemenu::brushButton;
        names[4] = Global::Elements::imagemenu::eraserButton;

        clickTexture[0].loadFromFile(BUTTON_TEXTURE_PATH("newClicked.png"));
        texture[0].loadFromFile(BUTTON_TEXTURE_PATH("new.png"));

        clickTexture[1].loadFromFile(BUTTON_TEXTURE_PATH("openClicked.png"));
        texture[1].loadFromFile(BUTTON_TEXTURE_PATH("open.png"));

        clickTexture[2].loadFromFile(BUTTON_TEXTURE_PATH("saveClicked.png"));
        texture[2].loadFromFile(BUTTON_TEXTURE_PATH("save.png"));

        clickTexture[3].loadFromFile(BUTTON_TEXTURE_PATH("brushClicked.png"));
        texture[3].loadFromFile(BUTTON_TEXTURE_PATH("brush.png"));

        clickTexture[4].loadFromFile(BUTTON_TEXTURE_PATH("eraserClicked.png"));
        texture[4].loadFromFile(BUTTON_TEXTURE_PATH("eraser.png"));


        for (int i = 0; i < buttonCount; i++)
        {
            ImgButton newButton(clickTexture[i], texture[i]);
            newButton.getButton()->setPosition(offset * (i + 1) + (32 * i), offset);

            imgButtons[names[i]] = newButton;
        }


        //Adds all buttons with corresponding names
        for (auto & b : imgButtons)
        {
            panel->add(b.second, b.first);
        }

        gui.add(panel, Global::Elements::imagemenu::panel);
    }

    //MENU BAR
    {
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
        bar->addMenuItem("Edit", Global::Elements::Menu::Clickables::redo);
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

        gui.add(bar, Global::Elements::Menu::bar);
    }

    //INFO BOX
    {
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
    }

    //Texture importer list
    {
        tgui::ListBox::Ptr textureList = tgui::ListBox::create();
        textureList->hide();
        textureList->setSize(400, 200);
        textureList->setPosition(400, 20);

        gui.add(textureList, Global::Elements::textureImporter::textureList);
    }

    //SAVE PANEL
    {
        tgui::Panel::Ptr savePanel = tgui::Panel::create();
        savePanel->setSize(500, 300);
        savePanel->setPosition(200, TOTAL_BAR_HEIGHT + DISTANCE);

        tgui::ListBox::Ptr savePathList = tgui::ListBox::create();
        savePathList->setPosition(10, 10);
        savePathList->setSize(200, 200);


        tgui::Button::Ptr folderButton = tgui::Button::create();
        folderButton->setPosition(220, 10);
        folderButton->setTextSize(BUTTON_TEXT_SIZE);
        folderButton->setSize(90, 25);
        folderButton->setText("New folder");

        tgui::Button::Ptr save = tgui::Button::create();
        save->setPosition(220, 220);
        save->setTextSize(BUTTON_TEXT_SIZE);
        save->setSize(70, 25);
        save->setText("Save");

        tgui::Button::Ptr exitButton = tgui::Button::create();
        exitButton->setPosition(220, 250);
        exitButton->setTextSize(BUTTON_TEXT_SIZE);
        exitButton->setSize(70, 25);
        exitButton->setText("Exit");

        tgui::TextBox::Ptr fileName = tgui::TextBox::create();
        fileName->setPosition(10, 220);
        fileName->setSize(200, 25);

        savePanel->add(savePathList, Global::Elements::savebox::paths);
        savePanel->add(folderButton, Global::Elements::savebox::createFolderButton);
        savePanel->add(save, Global::Elements::savebox::saveButton);
        savePanel->add(exitButton, Global::Elements::savebox::cancelButton);
        savePanel->add(fileName, Global::Elements::savebox::fileName);
        savePanel->hide();

        //      SAVE PANEL/CREATEFOLDER
        {
            tgui::Panel::Ptr folderPanel = tgui::Panel::create();
            folderPanel->setSize(220, 100);
            folderPanel->setPosition(215, 40);
            folderPanel->hide();

            tgui::TextBox::Ptr textBox = tgui::TextBox::create();
            textBox->setPosition(5, 5);
            textBox->setSize(120, 25);

            tgui::Button::Ptr confirm = tgui::Button::create();
            confirm->setPosition(135, 5);
            confirm->setTextSize(BUTTON_TEXT_SIZE);
            confirm->setSize(70, 25);
            confirm->setText("Confirm");

            tgui::Button::Ptr cancel = tgui::Button::create();
            cancel->setPosition(135, 35);
            cancel->setTextSize(BUTTON_TEXT_SIZE);
            cancel->setSize(70, 25);
            cancel->setText("Cancel");

            folderPanel->add(textBox, Global::Elements::savebox::foldercreator::textbox);
            folderPanel->add(confirm, Global::Elements::savebox::foldercreator::confirmButton);
            folderPanel->add(cancel, Global::Elements::savebox::foldercreator::cancelButton);

            savePanel->add(folderPanel, Global::Elements::savebox::foldercreator::panel);

        }

        gui.add(savePanel, Global::Elements::savebox::panel);
    }

    //LOAD PANEL
    {
        tgui::Panel::Ptr loadPanel = tgui::Panel::create();
        loadPanel->setSize(500, 300);
        loadPanel->setPosition(200, TOTAL_BAR_HEIGHT + DISTANCE);

        tgui::ListBox::Ptr loadPathList = tgui::ListBox::create();
        loadPathList->setPosition(10, 10);
        loadPathList->setSize(200, 200);

        tgui::Button::Ptr load = tgui::Button::create();
        load->setPosition(220, 220);
        load->setTextSize(BUTTON_TEXT_SIZE);
        load->setSize(70, 25);
        load->setText("Load");

        tgui::Button::Ptr exitButton = tgui::Button::create();
        exitButton->setPosition(220, 250);
        exitButton->setTextSize(BUTTON_TEXT_SIZE);
        exitButton->setSize(70, 25);
        exitButton->setText("Exit");

        loadPanel->add(loadPathList, Global::Elements::loadbox::paths);
        loadPanel->add(load, Global::Elements::loadbox::loadButton);
        loadPanel->add(exitButton, Global::Elements::loadbox::cancelButton);
        loadPanel->hide();

        gui.add(loadPanel, Global::Elements::loadbox::panel);
    }




    for (auto & b : imgButtons)
    {
        b.second.getButton()->connect("clicked",
            [&]()
        {
            b.second.swap();
        });
    }

    //cases where buttons should swap back to unpressed
    //Load icon
    auto label = gui.get<tgui::Panel>(Global::Elements::loadbox::panel);
    label->get(Global::Elements::loadbox::cancelButton)->connect("clicked", [&]() 
    {
        if (imgButtons[Global::Elements::imagemenu::openButton])
            imgButtons[Global::Elements::imagemenu::openButton].swap();
    });

    label->get(Global::Elements::loadbox::loadButton)->connect("clicked", [&]()
    {
        if (imgButtons[Global::Elements::imagemenu::openButton])
            imgButtons[Global::Elements::imagemenu::openButton].swap();
    });


    //Save icon
    label = gui.get<tgui::Panel>(Global::Elements::savebox::panel);
    label->get(Global::Elements::savebox::cancelButton)->connect("clicked", [&]()
    {
        if (imgButtons[Global::Elements::imagemenu::saveButton])
            imgButtons[Global::Elements::imagemenu::saveButton].swap();
    });

    label->get(Global::Elements::savebox::saveButton)->connect("clicked", [&]()
    {
        if (imgButtons[Global::Elements::imagemenu::saveButton])
            imgButtons[Global::Elements::imagemenu::saveButton].swap();
    });

    //Eraser
    label = gui.get<tgui::Panel>(Global::Elements::imagemenu::panel);
    label->get(Global::Elements::imagemenu::eraserButton)->connect("clicked", [&]()
    {
        if (imgButtons[Global::Elements::imagemenu::brushButton])
            imgButtons[Global::Elements::imagemenu::brushButton].swap();
    });

    //Brush
    label->get(Global::Elements::imagemenu::brushButton)->connect("clicked", [&]()
    {
        if (imgButtons[Global::Elements::imagemenu::eraserButton])
            imgButtons[Global::Elements::imagemenu::eraserButton].swap();
    });
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