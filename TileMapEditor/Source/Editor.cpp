#include "Editor.h"

Editor::Editor()
{

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
        }


        //update
        tileMenuHandler.update();




        //draw
        tileManager.prepareTiles();

        window.clear();
        window.draw(tileMenuHandler);
        window.draw(tileManager);
        window.display();

    }

    return 0;
}
