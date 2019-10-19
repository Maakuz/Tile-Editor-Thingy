#pragma once
#include "TGUI/TGUI.hpp"
namespace Global
{
    extern tgui::Gui * gui;
    
    namespace Elements
    {
        namespace infoBox
        {
            const sf::String panel("InfoPanel");

            const sf::String layerInfo("layerInfo");
            const sf::String textureBox("selectedTexture");
        
        }


        namespace resizeMenu
        {
            const sf::String panel("resizePanel");
            const sf::String width("canvasHeight");
            const sf::String confirm("canvasConfirm");
            const sf::String cancel("canvasCancel");
            const sf::String height("canvasWidth");

        }

        namespace textureImporter
        {
            const sf::String panel("TexturePanel");
            const sf::String textureList("listBox");
        }
    }
}