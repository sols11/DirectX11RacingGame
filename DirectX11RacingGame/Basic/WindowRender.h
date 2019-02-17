#pragma once
#include "ErrorLogger.h"

class WindowContent;

class WindowRender
{
public:
    bool Initialize(WindowContent* pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
    bool ProcessMessages();
    HWND GetHWND() const;
    ~WindowRender();
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