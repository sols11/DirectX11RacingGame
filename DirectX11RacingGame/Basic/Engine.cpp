#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
        return false;

    if (!this->graphics.Initialize(this->render_window.GetHWND(), width, height))
        return false;
}

bool Engine::ProcessMessages()
{
    return this->render_window.ProcessMessages();
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
