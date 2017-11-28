#include "Editor.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Time to tile!");

    Editor editor;

    return editor.run(window);
}