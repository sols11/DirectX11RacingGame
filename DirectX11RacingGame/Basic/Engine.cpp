#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    if (!this->windowRender.Initialize(this, hInstance, windowTitle, windowClass, width, height))
        return false;

    if (!this->graphics.Initialize(this->windowRender.GetHWND(), width, height))
        return false;

    return true;
}

bool Engine::ProcessMessages()
{
    return this->windowRender.ProcessMessages();
}

void Engine::Update()
{
    while (!keyboard.CharBufferIsEmpty())
    {
        unsigned char ch = keyboard.ReadChar();
        std::string outmsg = "Char:";
        outmsg += ch;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }

    while (!keyboard.KeyBufferIsEmpty())
    {
        KeyboardEvent kbe = keyboard.ReadKey();
        unsigned char keycode = kbe.GetKeycode();
        std::string outmsg = "";
        if (kbe.IsPress())
        {
            outmsg += "Key Press:";
        }
        if (kbe.IsRelease())
        {
            outmsg += "Key Release";
        }
        outmsg += keycode;
        outmsg += "\n";
        OutputDebugStringA(outmsg.c_str());
    }
}

void Engine::RenderFrame()
{
    this->graphics.RenderFrame();
}
