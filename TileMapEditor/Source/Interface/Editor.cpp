#include "Editor.h"
#include "GUI.h"

Editor::Editor(sf::RenderWindow & window) :
    gui(window),
    tileMenuHandler()
{
    //Oneliner aw yis
    Global::gui->get<tgui::MenuBar>(Global::Elements::Menu::bar)->connect("MenuItemClicked", [&](sf::String item) { if (item == Global::Elements::Menu::Clickables::exitFile) window.close(); });
   
}

int Editor::run(sf::RenderWindow & window)
{

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvents(event);

            if (!gui.isActive())
                tileMenuHandler.handleEvent(event);
        }


        //update
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        
        gui.update();

        if (!gui.isActive())
            tileMenuHandler.update(mousePos);

        tileMenuHandler.queueItems();
        renderer.update();
        //draw

        window.draw(renderer);
        gui.drawGui();
        window.display();
        window.clear(sf::Color(25, 100, 200));
        renderer.clearAllQueues();
    }

    return 0;
}