#pragma once
#include "WindowContent.h"
#include "Timer.h"

class Engine : WindowContent
{
public:
    bool Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
    bool ProcessMessages();
    void Update();
    void RenderFrame();
private:
    Timer timer;
};