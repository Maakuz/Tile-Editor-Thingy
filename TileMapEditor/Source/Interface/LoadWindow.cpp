#include "LoadWindow.h"
#include "GUI.h"
#include "Constants.h"

LoadWindow::LoadWindow()
{
    loading = false;

    auto panel = Global::gui->get<tgui::Panel>(Global::Elements::loadbox::panel);

    panel->get(Global::Elements::loadbox::paths)->connect("DoubleClicked", &LoadWindow::iteratePaths, this);
    panel->get(Global::Elements::loadbox::cancelButton)->connect("Clicked", &LoadWindow::closeWindow, this);
}

void LoadWindow::openWindow()
{
    loading = true;
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::loadbox::panel);
    window->show();

    currentDir = fs::current_path();

    printPaths();
}

void LoadWindow::closeWindow()
{
    loading = false;
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::loadbox::panel);
    window->hide();
}

void LoadWindow::printPaths()
{
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::loadbox::panel);

    auto fileList = window->get<tgui::ListBox>(Global::Elements::loadbox::paths);
    fileList->removeAllItems();

    for (auto & p : fs::directory_iterator(currentDir))
    {
        if (p.path().extension() == FILE_EXTENSION)
            fileList->addItem(p.path().filename().string(), p.path().string());

        else if (p.path().extension() == "")
            fileList->addItem(p.path().filename().string(), p.path().string());
    }

    fileList->addItem("..", currentDir.parent_path().string());

    printf("%s\n", currentDir.string().c_str());
}

void LoadWindow::iteratePaths(sf::String name, sf::String path)
{
    if (!(currentDir.root_path() == currentDir && name == ".."))
    {
        currentDir = std::string(path);
        if (currentDir.extension() != "")
            currentDir = currentDir.parent_path();
    }

    printPaths();
}