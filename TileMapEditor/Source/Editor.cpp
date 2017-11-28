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
        tileMenuHandler.update(window);



        renderer.update();
        //draw

        window.draw(renderer);
        window.display();
        window.clear();
        renderer.clearAllQueues();
    }

    return 0;
}
