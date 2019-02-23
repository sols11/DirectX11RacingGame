#pragma once
#include "ErrorLogger.h"
#include "..\Input\Keyboard.h"
#include "..\Input\Mouse.h"
#include "..\Graphics\Graphics.h"

class WindowContent
{
public:
    WindowContent();
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool ProcessMessages();
    HWND GetHWND() const;
    ~WindowContent();
protected:
    bool virtual Initialize(WindowContent* pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
    Keyboard keyboard;
    Mouse mouse;
    Graphics graphics;
private:
    void RegisterWindowClass(); // ×¢²á´°¿ÚÀà
    HWND handle = NULL;
    HINSTANCE hInstance = NULL;
    std::string windowTitle = "";
    std::wstring windowTitleWide = L"";
    std::string windowClass = "";
    std::wstring windowClassWide = L"";
    int width = 0;
    int height = 0;
};