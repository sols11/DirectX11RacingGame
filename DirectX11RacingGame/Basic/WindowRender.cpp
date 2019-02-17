#include "WindowContent.h"

// 初始化并显示窗口
bool WindowRender::Initialize(WindowContent * pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    this->hInstance = hInstance;
    this->width = width;
    this->height = height;
    this->window_title = window_title;
    this->window_title_wide = StringConverter::StringToWide(this->window_title);
    this->window_class = window_class;
    this->window_class_wide = StringConverter::StringToWide(this->window_class);

    this->RegisterWindowClass();

    this->handle = CreateWindowEx(0, this->window_class_wide.c_str(), this->window_title_wide.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
        0, 0, this->width, this->height, NULL, NULL, this->hInstance, pWindowContainer);

    if (this->handle == NULL)
    {
        ErrorLogger::Log(GetLastError(), "CreateWindowEX Failed for window: " + this->window_title);
        return false;
    }

    // 将窗口显示在屏幕上，并将其设置为主焦点
    ShowWindow(this->handle, SW_SHOW);
    SetForegroundWindow(this->handle);
    SetFocus(this->handle);

    return true;
}

// 消息处理
bool WindowRender::ProcessMessages()
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
            UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
            return false;
        }
    }

    return true;
}

HWND WindowRender::GetHWND() const
{
    return this->handle;
}

WindowRender::~WindowRender()
{
    if (this->handle != NULL)
    {
        UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
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
        WindowContent * pWindow = reinterpret_cast<WindowContent*>(pCreate->lpCreateParams);
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
void WindowRender::RegisterWindowClass()
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
    wc.lpszClassName = this->window_class_wide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
}