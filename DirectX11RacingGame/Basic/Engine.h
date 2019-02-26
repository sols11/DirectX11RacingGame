#pragma once
#include "WindowContent.h"
#include "Timer.h"

class Engine : public WindowContent
{
public:
    bool Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height);
    void Update();
    void RenderFrame();
private:
    Timer timer;
    float cameraSpeed = 0.006f;
    float carSpeed = 0.003f;
    float carRotSpeed = 0.003f;
};