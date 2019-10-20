#include "Editor.h"
#include "Imgui/SFML-imgui/imgui-SFML.h"
#include "Imgui/imgui.h"
#include "Constants.h"
#include "Lighting/LightQueue.h"

#define SCROLL_SPEED 2

Editor::Editor(sf::RenderWindow & window) :
    tileMenuHandler()
{
    this->workView = sf::View(sf::FloatRect(0, 0, WIN_WIDTH, WIN_HEIGHT));


    this->toolView = sf::View(sf::FloatRect(0, 0, TILEMENU_WIDTH, WIN_HEIGHT));
    this->toolView.setViewport(sf::FloatRect(0, 0, (float)(TILEMENU_WIDTH) / WIN_WIDTH, 1));
    this->zoom = 1;
}

int Editor::run(sf::RenderWindow & window)
{

    while (window.isOpen())
    {
        //update
        sf::Time deltaTime = deltaTimer.restart();
        float dt = deltaTime.asMilliseconds();

        static long long autoSaveCounter = 0;
        autoSaveCounter += dt;

        if (autoSaveCounter > 100000)
        {
            autoSaveCounter = 0;
            tileMenuHandler.autosave();
        }

        this->workView.setSize(WIN_WIDTH * zoom, WIN_HEIGHT * zoom);
        window.setView(this->workView);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2i workSpaceMousePos = (sf::Vector2i)window.mapPixelToCoords(mousePos);
        bool guiActive = ImGui::IsAnyItemHovered() || ImGui::IsAnyWindowHovered();
        
        //Event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelMoved)
                this->zoom -= (event.mouseWheelScroll.wheel / 10.f);

            if (zoom < 1)
                zoom = 1;
            
            ImGui::SFML::ProcessEvent(event);

            tileMenuHandler.handleEvent(event, guiActive, workSpaceMousePos);
        }
        scrollScreen(dt);

        ImGui::SFML::Update(window, deltaTime);


            tileMenuHandler.update(mousePos, workSpaceMousePos, guiActive);
            //printf("%d, %d\n", workSpaceMousePos.x, workSpaceMousePos.y);

        tileMenuHandler.queueItems(this->workView);
        renderer.update();


        //draw
        window.setView(this->workView);
        window.draw(renderer);

        if (tileMenuHandler.isRenderingLights())
            lightRenderer.renderLights(window, zoom, sf::BlendMultiply);

        window.setView(this->toolView);
        window.draw(renderer);
        ImGui::SFML::Render(window);
       
        window.display();
        window.clear(sf::Color(25, 100, 200));
        renderer.clearAllQueues();
    }

    return 0;
}

void Editor::scrollScreen(float dt)
{
    float scrollSpeed = SCROLL_SPEED * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        this->workView.move(scrollSpeed, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        this->workView.move(-scrollSpeed, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        this->workView.move(0, scrollSpeed);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        this->workView.move(0, -scrollSpeed);

    //prefer not to speak or do this
    if (this->workView.getCenter().x - this->workView.getSize().x / 2 - TILEMENU_WIDTH + (TILEMENU_WIDTH * zoom) < 0)
        this->workView.setCenter(((this->workView.getSize().x / 2) + TILEMENU_WIDTH) - (TILEMENU_WIDTH * zoom), this->workView.getCenter().y);

    if (this->workView.getCenter().y - this->workView.getSize().y / 2 - MENU_BAR_HEIGHT + (MENU_BAR_HEIGHT * zoom) < 0)
        this->workView.setCenter(this->workView.getCenter().x, this->workView.getSize().y / 2 + MENU_BAR_HEIGHT - (MENU_BAR_HEIGHT * zoom));
}
