#pragma once
#include "WindowContent.h"

class Engine : WindowContent
{
public:
    bool Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
    bool ProcessMessages();
    void Update();
    void RenderFrame();
};