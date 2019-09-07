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
                const sf::String resize("Resize canvas");
                const sf::String importTexture("Import texture");

                //TODO: set this to NROFLAYERS somehow
                const sf::String layers[3] = { "Layer 1", "Layer 2", "Layer 3" };

                const sf::String infoBox("Info box");
                const sf::String darken("Layer differences");

                const sf::String exportLayers("Export...");



            }

        }

        namespace resizeMenu
        {
            const sf::String panel("resizePanel");
            const sf::String width("canvasHeight");
            const sf::String confirm("canvasConfirm");
            const sf::String cancel("canvasCancel");
            const sf::String height("canvasWidth");

        }

        namespace imagemenu
        {
            const sf::String panel("imagePanel");
            const sf::String newButton("newB");
            const sf::String openButton("openB");
            const sf::String saveButton("saveB");
            const sf::String brushButton("brushB");
            const sf::String eraserButton("eraserB");

        }

        namespace textureImporter
        {
            const sf::String panel("TexturePanel");
            const sf::String textureList("listBox");
        }
    }
}