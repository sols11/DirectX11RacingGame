#include "WindowContent.h"

// ��ʼ������ʾ����
bool WindowRender::Initialize(WindowContent * pWindowContainer, HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
{
    this->hInstance = hInstance;
    this->width = width;
    this->height = height;
    this->windowTitle = windowTitle;
    this->windowTitleWide = StringConverter::StringToWstring(this->windowTitle);
    this->windowClass = windowClass;
    this->windowClassWide = StringConverter::StringToWstring(this->windowClass);

    this->RegisterWindowClass();

    this->handle = CreateWindowEx(0, this->windowClassWide.c_str(), this->windowTitleWide.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
        0, 0, this->width, this->height, NULL, NULL, this->hInstance, pWindowContainer);

    if (this->handle == NULL)
    {
        ErrorLogger::Log(GetLastError(), "CreateWindowEX Failed for window: " + this->windowTitle);
        return false;
    }

    // ��������ʾ����Ļ�ϣ�����������Ϊ������
    ShowWindow(this->handle, SW_SHOW);
    SetForegroundWindow(this->handle);
    SetFocus(this->handle);

    return true;
}

// ��Ϣ����
bool WindowRender::ProcessMessages()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    if (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    // ��鴰���Ƿ�ر�
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

HWND WindowRender::GetHWND() const
{
    return this->handle;
}

WindowRender::~WindowRender()
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
            // ����ptr��������
            WindowContent* const pWindow = reinterpret_cast<WindowContent*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            // ����Ϣת���������ദ�����
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

// ע�ᴰ����
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
    wc.lpszClassName = this->windowClassWide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
}