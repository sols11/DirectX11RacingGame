#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    timer.Start();

    if (!WindowContent::Initialize(this, hInstance, windowTitle, windowClass, width, height))
        return false;

    if (!this->graphics.Initialize(this->WindowContent::GetHWND(), width, height))
        return false;

    return true;
}

void Engine::Update()
{
    float dt = timer.GetMilisecondsElapsed();
    timer.Restart();

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

    while (!mouse.EventBufferIsEmpty())
    {
        MouseEvent me = mouse.ReadEvent();
        if (mouse.IsRightDown())
        {
            if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
            {
                this->graphics.camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
            }
        }
    }

    const float cameraSpeed = 0.006f;
    float objectSpeed = 0.003f;
    float objectRotSpeed = 0.003f;

    if (keyboard.KeyIsPressed('&'))
    {
        this->graphics.car.AdjustPosition(this->graphics.car.GetForwardVector() * objectSpeed * dt);
    }
    if (keyboard.KeyIsPressed('('))
    {
        this->graphics.car.AdjustPosition(this->graphics.car.GetBackwardVector() * objectSpeed * dt);
    }
    if (keyboard.KeyIsPressed('%'))
    {
        this->graphics.car.AdjustRotation(this->graphics.car.GetDownVector() * objectRotSpeed * dt);
    }
    if (keyboard.KeyIsPressed('\''))
    {
        this->graphics.car.AdjustRotation(this->graphics.car.GetUpVector() * objectRotSpeed * dt);
    }

    if (keyboard.KeyIsPressed('W'))
    {
        this->graphics.camera.AdjustPosition(this->graphics.camera.GetForwardVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('S'))
    {
        this->graphics.camera.AdjustPosition(this->graphics.camera.GetBackwardVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('A'))
    {
        this->graphics.camera.AdjustPosition(this->graphics.camera.GetLeftVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed('D'))
    {
        this->graphics.camera.AdjustPosition(this->graphics.camera.GetRightVector() * cameraSpeed * dt);
    }
    if (keyboard.KeyIsPressed(VK_SPACE))
    {
        this->graphics.camera.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
    }
    if (keyboard.KeyIsPressed('X'))
    {
        this->graphics.camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
    }
    // µÚÒ»ÈË³Æ
    //graphics.object.SetWorldMatrix(XMMatrixTranslation(graphics.camera.GetPositionFloat3().x, graphics.camera.GetPositionFloat3().y, graphics.camera.GetPositionFloat3().z));

}

void Engine::RenderFrame()
{
    this->graphics.RenderFrame();
}
