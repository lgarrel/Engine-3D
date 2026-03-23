#include "pch.h"

InputManager* InputManager::s_instance = nullptr;

InputManager* InputManager::GetInstance()
{
    if (!s_instance)
        s_instance = new InputManager();
    return s_instance;
}

void InputManager::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        if (wParam < 256) m_keys[wParam] = true;
        break;

    case WM_KEYUP:
        if (wParam < 256) m_keys[wParam] = false;
        break;

    case WM_MOUSEMOVE:
    {
        if (!m_cursorHidden)
        {
            int x = (int)(short)LOWORD(lParam);
            int y = (int)(short)HIWORD(lParam);
            m_mouseDeltaX = x - m_mouseX;
            m_mouseDeltaY = y - m_mouseY;
            m_mouseX = x;
            m_mouseY = y;
        }
        break;
    }

    case WM_LBUTTONDOWN:
        m_mouseButtons[0] = true;
        break;

    case WM_LBUTTONUP:
        m_mouseButtons[0] = false;
        break;

    case WM_RBUTTONDOWN:
        m_mouseButtons[1] = true;
        break;

    case WM_RBUTTONUP:
        m_mouseButtons[1] = false;
        break;
    }
}

void InputManager::Update()
{ 
    memcpy(m_prevKeys, m_keys, sizeof(m_keys));
    memcpy(m_prevMouseButtons, m_mouseButtons, sizeof(m_mouseButtons));

    UpdateMouse();
    UpdateGamepad();
}

void InputManager::UpdateMouse()
{
    if (!m_cursorHidden || !m_confineHwnd)
    {
        m_mouseDeltaX = 0;
        m_mouseDeltaY = 0;
        return;
    }

    
    RECT rect;
    GetClientRect(m_confineHwnd, &rect);

    POINT center = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };
    POINT screenCenter = center;
    ClientToScreen(m_confineHwnd, &screenCenter);

    POINT current;
    GetCursorPos(&current);

    m_mouseDeltaX = current.x - screenCenter.x;
    m_mouseDeltaY = current.y - screenCenter.y;

    if (m_mouseDeltaX != 0 || m_mouseDeltaY != 0)
        SetCursorPos(screenCenter.x, screenCenter.y);

    m_mouseX = center.x;
    m_mouseY = center.y;
}


bool InputManager::IsMouseButtonDown(int button) const
{
    if (button < 0 || button >= 2) 
        return false;

    return m_mouseButtons[button];
}

bool InputManager::IsMouseButtonPressed(int button) const
{
    if (button < 0 || button >= 2)
        return false;

    return m_mouseButtons[button] && !m_prevMouseButtons[button];
}


void InputManager::SetCursorVisible(bool visible)
{
    m_cursorHidden = !visible;
    if (visible)
        while (ShowCursor(TRUE) < 0); 
    else
        while (ShowCursor(FALSE) >= 0);
}

void InputManager::ConfineCursor(HWND hwnd)
{
    m_confineHwnd = hwnd;

    if (hwnd)
    {
        RECT rect;
        GetClientRect(hwnd, &rect);
        POINT center = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };

        POINT screenCenter = center;
        ClientToScreen(hwnd, &screenCenter);
        SetCursorPos(screenCenter.x, screenCenter.y);

        m_mouseX = center.x;
        m_mouseY = center.y;
        m_mouseDeltaX = 0;
        m_mouseDeltaY = 0;
    }
}

void InputManager::UpdateGamepad()
{ 
    if (m_gamepadConnected)
        m_prevButtons = m_gamepadState.Gamepad.wButtons;
    else
        m_prevButtons = 0;

    m_prevLeftTrigger = m_leftTrigger;
    m_prevRightTrigger = m_rightTrigger;

    DWORD dwResult = XInputGetState(0, &m_gamepadState);

    if (dwResult == ERROR_SUCCESS)
    {
        m_gamepadConnected = true;

        float normLX = fmaxf(-1, (float)m_gamepadState.Gamepad.sThumbLX / 32767);
        float normLY = fmaxf(-1, (float)m_gamepadState.Gamepad.sThumbLY / 32767);
        m_leftStickX = (abs(normLX) < 0.2f) ? 0.0f : normLX;
        m_leftStickY = (abs(normLY) < 0.2f) ? 0.0f : normLY;

        float normRX = fmaxf(-1, (float)m_gamepadState.Gamepad.sThumbRX / 32767);
        float normRY = fmaxf(-1, (float)m_gamepadState.Gamepad.sThumbRY / 32767);
        m_rightStickX = (abs(normRX) < 0.2f) ? 0.0f : normRX;
        m_rightStickY = (abs(normRY) < 0.2f) ? 0.0f : normRY;

        m_leftTrigger = m_gamepadState.Gamepad.bLeftTrigger / 255.f;
        m_rightTrigger = m_gamepadState.Gamepad.bRightTrigger / 255.f;
    }
    else
    {
        m_gamepadConnected = false;
        m_leftStickX = m_leftStickY = m_rightStickX = m_rightStickY = 0.0f;
        m_leftTrigger = m_rightTrigger = 0.0f;
    }
}

bool InputManager::IsGamepadButtonDown(WORD button) const
{
    if (!m_gamepadConnected) return false;
    return (m_gamepadState.Gamepad.wButtons & button) != 0;
}

bool InputManager::IsGamepadButtonPressed(WORD button) const
{
    if (!m_gamepadConnected) return false;
    return (m_gamepadState.Gamepad.wButtons & button) != 0 && (m_prevButtons & button) == 0;
}

bool InputManager::IsKeyDown(int key) const
{ 
    if (key < 0 || key >= 256) return false;
     
    return m_keys[key];
}

bool InputManager::IsKeyPressed(int key) const
{ 
    if (key < 0 || key >= 256) return false;
     
    return m_keys[key] && !m_prevKeys[key];
}