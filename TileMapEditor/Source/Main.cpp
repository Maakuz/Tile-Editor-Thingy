#include <Interface\Editor.h>
#include "Constants.h"

int main()
{
//#ifdef _MSVC_LANG
//    printf("_MSVC_LANG : C++%d \n", (_MSVC_LANG / 100) % 2000);
//#endif

#ifdef _DEBUG
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
    _CrtSetDbgFlag(flag);
    _CrtSetBreakAlloc(689); // Comment or un-comment on need basis
#endif

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Time to tile!");

    window.setFramerateLimit(120);

    Editor editor(window);

    return editor.run(window);
}