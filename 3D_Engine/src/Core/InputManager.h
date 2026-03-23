#pragma once
#include "pch.h"

class InputManager
{
public:
    static InputManager* GetInstance();
     
    void ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
     
    void Update();
     
    bool IsKeyDown(int key) const;      
    bool IsKeyPressed(int key) const;   
     
    int GetMouseX() const { return m_mouseX; }
    int GetMouseY() const { return m_mouseY; }
    int GetMouseDeltaX() const { return m_mouseDeltaX; }
    int GetMouseDeltaY() const { return m_mouseDeltaY; }

    bool IsMouseButtonDown(int button) const;      
    bool IsMouseButtonPressed(int button) const;    

    void SetCursorVisible(bool visible);
    void ConfineCursor(HWND hwnd);                 
    bool IsCursorHidden() const { return m_cursorHidden; }

    bool IsGamepadConnected() const { return m_gamepadConnected; }
    bool IsGamepadButtonDown(WORD button) const;
    bool IsGamepadButtonPressed(WORD button) const;
    float GetLeftStickX()  const { return m_leftStickX; }
    float GetLeftStickY()  const { return m_leftStickY; }
    float GetRightStickX() const { return m_rightStickX; }
    float GetRightStickY() const { return m_rightStickY; }

    float GetLeftTrigger()  const { return m_leftTrigger; }
    float GetRightTrigger() const { return m_rightTrigger; }

    bool IsLeftTriggerPressed(float threshold = 0.5f) { return m_leftTrigger >= threshold && m_prevLeftTrigger < threshold; }
    bool IsRightTriggerPressed(float threshold = 0.5f) { return m_rightTrigger >= threshold && m_prevRightTrigger < threshold; }


private:
    InputManager() = default;
    static InputManager* s_instance;

    bool m_keys[256] = { false };
    bool m_prevKeys[256] = { false };

    int m_mouseX = 0;
    int m_mouseY = 0;
    int m_mouseDeltaX = 0;
    int m_mouseDeltaY = 0;
    bool m_mouseButtons[2] = { false };
    bool m_prevMouseButtons[2] = { false };
    POINT m_centerPos = {};

    bool  m_cursorHidden = false;
    HWND  m_confineHwnd = nullptr;

    void UpdateMouse();
    void UpdateGamepad();

    XINPUT_STATE m_gamepadState = {};
    bool m_gamepadConnected = false;
    WORD m_prevButtons = 0;

    float m_leftStickX = 0.0f;
    float m_leftStickY = 0.0f;
    float m_rightStickX = 0.0f;
    float m_rightStickY = 0.0f;

    float m_leftTrigger = 0.0f;
    float m_rightTrigger = 0.0f;
    float m_prevLeftTrigger = 0.0f;
    float m_prevRightTrigger = 0.0f;
};