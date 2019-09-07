#include <Interface\Editor.h>
#include "Constants.h"

int main()
{
//#ifdef _MSVC_LANG
//    printf("_MSVC_LANG : C++%d \n", (_MSVC_LANG / 100) % 2000);
//#endif

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Time to tile!");

    window.setFramerateLimit(120);

    Editor editor(window);

    return editor.run(window);
}