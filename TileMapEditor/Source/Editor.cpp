#include "Editor.h"
#include "TileQueue.h"

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
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                Tile test;
                test.textureID = 0;
                test.tileID = j + i * 10;
                test.x = i;
                test.y = j;

                TileQueue::get().queue(test);
            }
        }




        //draw
        tm.prepareTiles();

        window.clear();
        window.draw(tm);
        window.display();

    }

    return 0;
}
