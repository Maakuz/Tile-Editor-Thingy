#include "Editor.h"
#include "GUI.h"
#include "Constants.h"

#define SCROLL_SPEED 1

Editor::Editor(sf::RenderWindow & window) :
    gui(window),
    tileMenuHandler()
{
    //Oneliner aw yis
    Global::gui->get<tgui::MenuBar>(Global::Elements::Menu::bar)->connect("MenuItemClicked", [&](sf::String item) { if (item == Global::Elements::Menu::Clickables::exitFile) window.close(); });
   
    this->workView = sf::View(sf::FloatRect(0, 0, WIN_WIDTH, WIN_HEIGHT));


    this->toolView = sf::View(sf::FloatRect(0, 0, TILEMENU_WIDTH, WIN_HEIGHT));
    this->toolView.setViewport(sf::FloatRect(0, 0, (float)(TILEMENU_WIDTH) / WIN_WIDTH, 1));
}

int Editor::run(sf::RenderWindow & window)
{

    while (window.isOpen())
    {
        //update
        float dt = deltaTimer.restart().asMilliseconds();

        window.setView(this->workView);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2i workSpaceMousePos = (sf::Vector2i)window.mapPixelToCoords(mousePos);

#pragma region event_loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvents(event);

            tileMenuHandler.handleEvent(event, gui.isActive(), workSpaceMousePos);
        }
#pragma endregion 

        float scrollSpeed = SCROLL_SPEED * dt;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            this->workView.move(scrollSpeed, 0);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            this->workView.move(-scrollSpeed, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            this->workView.move(0, scrollSpeed);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            this->workView.move(0, -scrollSpeed);

        if (this->workView.getCenter().x < WIN_WIDTH / 2)
            this->workView.setCenter(WIN_WIDTH / 2, this->workView.getCenter().y);

        if (this->workView.getCenter().y < WIN_HEIGHT / 2)
            this->workView.setCenter(this->workView.getCenter().x, WIN_HEIGHT / 2);

        gui.update();

        if (!gui.isActive())
        {

            tileMenuHandler.update(mousePos, workSpaceMousePos);
            //printf("%f, %f\n", window.mapPixelToCoords(mousePos).x, window.mapPixelToCoords(mousePos).y);
        }

        tileMenuHandler.queueItems(this->workView);
        renderer.update();


        //draw
        window.setView(this->workView);
        window.draw(renderer);

        window.setView(this->toolView);
        window.draw(renderer);
        gui.drawGui();
       
        window.display();
        window.clear(sf::Color(25, 100, 200));
        renderer.clearAllQueues();
    }

    return 0;
}