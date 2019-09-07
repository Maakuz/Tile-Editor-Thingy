#pragma once
#include "SFML\System\String.hpp"

class ResizeWindow
{
public:
    ResizeWindow();
    virtual ~ResizeWindow() {};

    bool isOpen() const { return open; };
    void openWindow();
    void closeWindow();

private:

    bool open;
};