#pragma once
#include "TGUI\TGUI.hpp"
namespace Global
{
    extern tgui::Gui * gui;
    
    namespace Elements
    {
        namespace infoBox
        {
            const sf::String panel("InfoPanel");

            const sf::String layerInfo("layerInfo");
        
        }


        namespace Menu
        {
            const sf::String bar("Menu");

            namespace Clickables
            {
                const sf::String newFile("New...");
                const sf::String openFile("Open...");
                const sf::String saveFile("Save...");
                const sf::String exitFile("Exit");

                const sf::String undo("Undo");

                const sf::String layer1("Layer 1");
                const sf::String layer2("Layer 2");
                const sf::String layer3("Layer 3");

                const sf::String infoBox("Info box");

                const sf::String exportLayers("Export...");



            }

        }
    }
}