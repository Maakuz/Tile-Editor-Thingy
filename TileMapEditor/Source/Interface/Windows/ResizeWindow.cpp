#include "ResizeWindow.h"
#include "GUI.h"

ResizeWindow::ResizeWindow()
{
    this->open = false;

    auto panel = Global::gui->get<tgui::Panel>(Global::Elements::resizeMenu::panel);

    panel->get(Global::Elements::resizeMenu::cancel)->connect("Clicked", &ResizeWindow::closeWindow, this);

}

void ResizeWindow::openWindow()
{
    this->open = true;
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::resizeMenu::panel);
    window->show();
}

void ResizeWindow::closeWindow()
{
    this->open = false;
    auto window = Global::gui->get<tgui::Panel>(Global::Elements::resizeMenu::panel);
    window->hide();
}
