#include "SaveWindow.h"
#include "GUI.h"
#include "Constants.h"


SaveWindow::SaveWindow()
{
    saving = false;
    creatingFolder = false;

    auto panel = Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel);
    
    panel->get(Global::Elements::savebox::paths)->connect("DoubleClicked", &SaveWindow::iteratePaths, this);
    panel->get(Global::Elements::savebox::createFolderButton)->connect("Clicked", &SaveWindow::showFolderCreationWindow, this);
    panel->get(Global::Elements::savebox::cancelButton)->connect("Clicked", &SaveWindow::closeWindow, this);

    auto folderPanel = panel->get<tgui::Panel>(Global::Elements::savebox::foldercreator::panel);
    folderPanel->get(Global::Elements::savebox::foldercreator::cancelButton)->connect("Clicked", &SaveWindow::cancelFolderCreation, this);
    folderPanel->get(Global::Elements::savebox::foldercreator::confirmButton)->connect("Clicked", &SaveWindow::createFolder, this);

}

void SaveWindow::openWindow()
{
    saving = true;
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel);
    window->show();

    currentDir = fs::current_path();

    printPaths();
}

void SaveWindow::closeWindow()
{
    saving = false;
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel);
    window->hide();
}

fs::path SaveWindow::getPath() const
{
    return currentDir;
}

void SaveWindow::printPaths()
{
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel);

    auto fileList = window->get<tgui::ListBox>(Global::Elements::savebox::paths);
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

void SaveWindow::iteratePaths(sf::String name, sf::String path)
{
    if (creatingFolder)
        return;

    if (!(currentDir.root_path() == currentDir && name == ".."))
    {
        currentDir = std::string(path);
        if (currentDir.extension() != "")
            currentDir = currentDir.parent_path();
    }

    printPaths();
}

void SaveWindow::showFolderCreationWindow()
{
    creatingFolder = true;
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel);
    auto panel = window->get<tgui::Panel>(Global::Elements::savebox::foldercreator::panel);

    panel->show();
}

void SaveWindow::cancelFolderCreation()
{
    creatingFolder = false;
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel);
    auto panel = window->get<tgui::Panel>(Global::Elements::savebox::foldercreator::panel);

    panel->hide();
}

void SaveWindow::createFolder()
{
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::savebox::panel);
    auto panel = window->get<tgui::Panel>(Global::Elements::savebox::foldercreator::panel);

    sf::String text = panel->get<tgui::TextBox>(Global::Elements::savebox::foldercreator::textbox)->getText();

    fs::path newPath = currentDir;
    newPath /= std::string(text);

    try
    {
        fs::create_directory(newPath);
    }
    catch (const std::exception& e)
    {
        printf("%s", e.what());
    }

    cancelFolderCreation();
    printPaths();
}
