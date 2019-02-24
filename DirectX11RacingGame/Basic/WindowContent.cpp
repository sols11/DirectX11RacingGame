#include "WindowContent.h"

WindowContent::WindowContent()
{
    // 设置原始输入
    static bool rawInputInitialized = false;
    if (rawInputInitialized == false)
    {
        RAWINPUTDEVICE rid;
        
        rid.usUsagePage = 0x01; // Mouse
        rid.usUsage = 0x02;
        rid.dwFlags = 0;
        rid.hwndTarget = NULL;

        if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
        {
            ErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
            exit(-1);
        }

        rawInputInitialized = true;
    }
}

LRESULT WindowContent::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    // Keyboard Messages
    case WM_KEYDOWN:
    {
        unsigned char keycode = static_cast<unsigned char>(wParam);
        if (keyboard.IsKeysAutoRepeat())
        {
            keyboard.OnKeyPressed(keycode);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                keyboard.OnKeyPressed(keycode);
            }
        }
        return 0;
    }
    case WM_KEYUP:
    {
        unsigned char keycode = static_cast<unsigned char>(wParam);
        keyboard.OnKeyReleased(keycode);
        return 0;
    }
    case WM_CHAR:
    {
        unsigned char ch = static_cast<unsigned char>(wParam);
        if (keyboard.IsCharsAutoRepeat())
        {
            keyboard.OnChar(ch);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                keyboard.OnChar(ch);
            }
        }
        return 0;
    }
    // Mouse Messages
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnMouseMove(x, y);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnLeftPressed(x, y);
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnRightPressed(x, y);
        return 0;
    }
    case WM_MBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnMiddlePressed(x, y);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnLeftReleased(x, y);
        return 0;
    }
    case WM_RBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnRightReleased(x, y);
        return 0;
    }
    case WM_MBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnMiddleReleased(x, y);
        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
            mouse.OnWheelUp(x, y);
        }
        else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
            mouse.OnWheelDown(x, y);
        }
        return 0;
    }
    case WM_INPUT:
    {
        UINT dataSize;
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

        if (dataSize > 0)
        {
            std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
            {
                RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                }
            }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// 初始化并显示窗口
bool WindowContent::Initialize(WindowContent * pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    this->hInstance = hInstance;
    this->width = width;
    this->height = height;
    this->windowTitle = windowTitle;
    this->windowTitleWide = StringConverter::StringToWstring(this->windowTitle);
    this->windowClass = windowClass;
    this->windowClassWide = StringConverter::StringToWstring(this->windowClass);
    // 注册窗口类
    this->RegisterWindowClass();
    // 创建窗口
    this->handle = CreateWindowEx(0, this->windowClassWide.c_str(), this->windowTitleWide.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        0, 0, this->width, this->height, NULL, NULL, this->hInstance, pWindowContainer);
    if (this->handle == NULL)
    {
        ErrorLogger::Log(GetLastError(), "CreateWindowEX Failed for window: " + this->windowTitle);
        return false;
    }

    // 将窗口显示在屏幕上，并将其设置为主焦点
    ShowWindow(this->handle, SW_SHOW);
    SetForegroundWindow(this->handle);
    SetFocus(this->handle);

    return true;
}

// 消息处理
bool WindowContent::ProcessMessages()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    if (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 检查窗口是否关闭
    if (msg.message == WM_NULL)
    {
        if (!IsWindow(this->handle))
        {
            this->handle = NULL;
            UnregisterClass(this->windowClassWide.c_str(), this->hInstance);
            return false;
        }
    }

    return true;
}

HWND WindowContent::GetHWND() const
{
    return this->handle;
}

WindowContent::~WindowContent()
{
    if (this->handle != NULL)
    {
        UnregisterClass(this->windowClassWide.c_str(), this->hInstance);
        DestroyWindow(handle);
    }
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    default:
    {
        // 检索ptr到窗口类
        WindowContent* const pWindow = reinterpret_cast<WindowContent*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        // 将消息转发到窗口类处理程序
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NCCREATE:
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowContent* pWindow = reinterpret_cast<WindowContent*>(pCreate->lpCreateParams);
        if (pWindow == nullptr)
        {
            ErrorLogger::Log("Critical Error: Pointer to window container is null during WM_NCCREATE.");
            exit(-1);
        }
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// 注册窗口类
void WindowContent::RegisterWindowClass()
{
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = HandleMessageSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = this->hInstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = this->windowClassWide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
}