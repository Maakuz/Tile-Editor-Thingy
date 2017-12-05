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
            const sf::String textureBox("selectedTexture");
        
        }

        namespace savebox
        {
            const sf::String panel("savePanel");
            const sf::String paths("path");
            const sf::String fileName("fileName");
            const sf::String createFolderButton("New Folder");
            const sf::String saveButton("Save");
            const sf::String cancelButton("Cancel");

            namespace foldercreator
            {
                const sf::String panel("panel");
                const sf::String textbox("Folder name");
                const sf::String confirmButton("Confirm");
                const sf::String cancelButton("Cancel");

            }
        }

        namespace loadbox
        {
            const sf::String panel("loadPanel");
            const sf::String paths("path");
            const sf::String loadButton("Load");
            const sf::String cancelButton("Cancel");
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
                const sf::String redo("Redo");
                const sf::String importTexture("Import texture");

                const sf::String layer1("Layer 1");
                const sf::String layer2("Layer 2");
                const sf::String layer3("Layer 3");

                const sf::String infoBox("Info box");
                const sf::String darken("Layer differences");

                const sf::String exportLayers("Export...");



            }

        }

        namespace imagemenu
        {
            const sf::String panel("imagePanel");
            const sf::String newButton("newB");
            const sf::String openButton("openB");
            const sf::String saveButton("saveB");

        }

        namespace textureImporter
        {
            const sf::String panel("TexturePanel");
            const sf::String textureList("listBox");
        }
    }
}