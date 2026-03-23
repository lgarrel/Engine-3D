#include "pch.h"
#include "../Render/GraphicsEngine.h"

Window::Window(int width, int height, std::wstring name)
    : mWidth(width), mHeight(height), mName(name) 
{
}


LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
    InputManager::GetInstance()->ProcessMessage(uMsg, wParam, lParam);

    if (uMsg == WM_DESTROY) 
    {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


bool Window::Initialize() 
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = Window::WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"DX12WindowClass";

    RegisterClass(&wc);

    mHwnd = CreateWindowEx(0, wc.lpszClassName, mName.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        mWidth, mHeight, nullptr, nullptr, wc.hInstance, nullptr);

    if (!mHwnd) 
        return false;

    InputManager* input = InputManager::GetInstance();
    input->SetCursorVisible(false);
    input->ConfineCursor(mHwnd);

    ShowWindow(mHwnd, SW_SHOW);

    return true;
}

void Window::Run(GraphicsEngine* engine) 
{
    MSG msg = {};
    Timer timer;
    timer.Reset();

    while (mIsRunning) 
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT) mIsRunning = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (mIsRunning) 
        {
            timer.Tick();
            InputManager* input = InputManager::GetInstance();

            if (input->IsKeyPressed('E') || input->IsGamepadButtonPressed(XINPUT_GAMEPAD_B))
            {
                mIsSlowMo = !mIsSlowMo;
            }

            if (input->IsKeyPressed('F') || input->IsGamepadButtonPressed(XINPUT_GAMEPAD_A))
            {
                mIsFast = !mIsFast;
            }

            if (input->IsKeyPressed(VK_ESCAPE))
            {
                if (input->IsCursorHidden())
                {
                    input->SetCursorVisible(true);
                    input->ConfineCursor(nullptr);
                }
                else
                {
                    input->SetCursorVisible(false);
                    input->ConfineCursor(mHwnd);
                }
            }

            float dt = timer.DeltaTime();
            if (mIsSlowMo) 
            {
                dt *= 0.1f;
            }
            if (mIsFast)
            {
                dt *= 2.f;
            }

            engine->Update(dt);
            engine->Render();

            input->Update();
        }
    }
}


Window::~Window() {}
