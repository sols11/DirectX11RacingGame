#pragma once
#include "WindowRender.h"
#include "..\Input\Keyboard.h"
#include "..\Graphics\Graphics.h"

class WindowContent
{
public:
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
    WindowRender render_window;
    Keyboard keyboard;
    Graphics graphics;
private:

};