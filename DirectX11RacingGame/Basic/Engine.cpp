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
    float deltaTime = timer.GetMilisecondsElapsed();
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

    if (keyboard.KeyIsPressed('1') && this->graphics.camera.mode != Camera::Mode::FirstPerson)
    {
        this->graphics.camera.mode = Camera::Mode::FirstPerson;
        this->graphics.camera.SetPosition(this->graphics.car.GetPositionVector());
        this->graphics.camera.SetRotation(this->graphics.car.GetRotationVector());
        this->graphics.car.dontDraw = true;
    }
    else if (keyboard.KeyIsPressed('2') && this->graphics.camera.mode != Camera::Mode::ThirdPerson)
    {
        this->graphics.camera.mode = Camera::Mode::ThirdPerson;
        this->graphics.car.SetPosition(this->graphics.camera.GetPositionVector());
        this->graphics.car.dontDraw = false;
    }

    while (!mouse.EventBufferIsEmpty())
    {
        MouseEvent mouseEvent = mouse.ReadEvent();
        if (mouse.IsRightDown())
        {
            if (mouseEvent.GetType() == MouseEvent::EventType::RAW_MOVE)
            {
                // 自由旋转
                if (this->graphics.camera.mode == Camera::Mode::Free)
                {
                    this->graphics.camera.AdjustRotation((float)mouseEvent.GetPosY() * 0.01f, (float)mouseEvent.GetPosX() * 0.01f, 0);
                }
                // 绕物体旋转
                else if (this->graphics.camera.mode == Camera::Mode::ThirdPerson)
                {
                    this->graphics.camera.RotateX((float)mouseEvent.GetPosY() * deltaTime * 0.01f);
                    this->graphics.camera.RotateY((float)mouseEvent.GetPosX() * deltaTime * 0.01f);
                }
            }
        }
    }

    // 自由移动相机
    if (this->graphics.camera.mode == Camera::Mode::Free)
    {
        if (keyboard.KeyIsPressed('W'))
        {
            this->graphics.camera.AdjustPosition(this->graphics.camera.GetForwardVector() * cameraSpeed * deltaTime);
        }
        if (keyboard.KeyIsPressed('S'))
        {
            this->graphics.camera.AdjustPosition(this->graphics.camera.GetForwardVector() * -cameraSpeed * deltaTime);
        }
        if (keyboard.KeyIsPressed('A'))
        {
            this->graphics.camera.AdjustPosition(this->graphics.camera.GetRightVector() * -cameraSpeed * deltaTime);
        }
        if (keyboard.KeyIsPressed('D'))
        {
            this->graphics.camera.AdjustPosition(this->graphics.camera.GetRightVector() * cameraSpeed * deltaTime);
        }
        if (keyboard.KeyIsPressed(VK_SPACE))
        {
            this->graphics.camera.AdjustPosition(0.0f, cameraSpeed * deltaTime, 0.0f);
        }
        if (keyboard.KeyIsPressed('X'))
        {
            this->graphics.camera.AdjustPosition(0.0f, -cameraSpeed * deltaTime, 0.0f);
        }
    }
    // 控制汽车移动
    else
    {
        if (keyboard.KeyIsPressed('W')) // ↑
        {
            if (this->graphics.camera.mode == Camera::Mode::FirstPerson)
            {
                this->graphics.camera.AdjustPosition(this->graphics.camera.GetForwardVector() * carSpeed * deltaTime);
            }
            else
            {
                this->graphics.car.WheelRoll(deltaTime);    // 要在car调用AdjustPosition之前调用
                this->graphics.car.AdjustPosition(this->graphics.car.GetForwardVector() * carSpeed * deltaTime);
            }
        }
        if (keyboard.KeyIsPressed('S')) // ↓
        {
            if (this->graphics.camera.mode == Camera::Mode::FirstPerson)
            {
                this->graphics.camera.AdjustPosition(this->graphics.camera.GetForwardVector() * -carSpeed * deltaTime);
            }
            else
            {
                this->graphics.car.WheelRoll(-deltaTime);
                this->graphics.car.AdjustPosition(this->graphics.car.GetForwardVector() * -carSpeed * deltaTime);
            }
        }
        if (keyboard.KeyIsPressed('A')) // ←
        {
            if (this->graphics.camera.mode == Camera::Mode::FirstPerson)
            {
                this->graphics.camera.AdjustRotation(this->graphics.camera.GetUpVector() * -carRotSpeed * deltaTime);
            }
            else
            {
                this->graphics.car.WheelRoll(deltaTime);
                this->graphics.car.AdjustRotation(this->graphics.car.GetUpVector() * -carRotSpeed * deltaTime);
            }
        }
        if (keyboard.KeyIsPressed('D')) // →
        {
            if (this->graphics.camera.mode == Camera::Mode::FirstPerson)
            {
                this->graphics.camera.AdjustRotation(this->graphics.camera.GetUpVector() * carRotSpeed * deltaTime);
            }
            else
            {
                this->graphics.car.WheelRoll(deltaTime);
                this->graphics.car.AdjustRotation(this->graphics.car.GetUpVector() * carRotSpeed * deltaTime);
            }
        }
    }
    // 最后再更新相机
    this->graphics.camera.UpdateViewMatrix();
}

void Engine::RenderFrame()
{
    this->graphics.RenderFrame();
}
