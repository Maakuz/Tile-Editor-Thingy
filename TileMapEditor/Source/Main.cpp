#include <Interface\Editor.h>

int main()
{
//#ifdef _MSVC_LANG
//    printf("_MSVC_LANG : C++%d \n", (_MSVC_LANG / 100) % 2000);
//#endif

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Time to tile!");

    Editor editor(window);

    return editor.run(window);
}