#pragma once
#include "WindowRender.h"
#include "..\Input\Keyboard.h"
#include "..\Input\Mouse.h"
#include "..\Graphics\Graphics.h"

class WindowContent
{
public:
    WindowContent();
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
    WindowRender windowRender;
    Keyboard keyboard;
    Mouse mouse;
    Graphics graphics;
private:

};