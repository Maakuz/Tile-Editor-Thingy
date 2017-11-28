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




        //draw

        window.clear();
        window.draw(tm);
        window.display();
        tm.clearQueue();

    }

    return 0;
}
