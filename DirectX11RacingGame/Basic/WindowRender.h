#pragma once
#include "ErrorLogger.h"

class WindowContent;

class WindowRender
{
public:
    bool Initialize(WindowContent* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
    bool ProcessMessages();
    HWND GetHWND() const;
    ~WindowRender();
private:
    void RegisterWindowClass(); // ×¢²á´°¿ÚÀà
    HWND handle = NULL;                                                                    
    HINSTANCE hInstance = NULL;
    std::string window_title = "";
    std::wstring window_title_wide = L"";
    std::string window_class = "";
    std::wstring window_class_wide = L"";
    int width = 0;
    int height = 0;
};