#pragma once


class GraphicsEngine;

class Window {
public:
    Window(int width, int height, std::wstring name = L"3D_Engine");
    ~Window();


    bool Initialize();
    void Run(GraphicsEngine* engine);

    HWND GetHandle() const { return mHwnd; }
    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }

private:

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND mHwnd = nullptr;
    int mWidth;
    int mHeight;
    std::wstring mName;
    bool mIsRunning = true;
    bool mIsSlowMo = false;
    bool mIsFast = false;

};