#include "pch.h"
#include "main.h"
#include "StartScene.h"
#include "TestGameplay.h"

#ifdef _DEBUG


#pragma comment(linker, "/SUBSYSTEM:CONSOLE")

int main()
{
    GameManager* gameManager = GameManager::GetInstance();

    srand(time(NULL));

    if (!gameManager->Initialize<StartScene>(WIN_WIDTH, WIN_HEIGHT, L"Engine DX12")) {
        return -1;
    }

    gameManager->Run();

    return 0;
}
#else
// OU forcer le mode fenetre (pas de console)
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

    GameManager* gameManager = GameManager::GetInstance();

    srand(time(NULL));

    if (!gameManager->Initialize<StartScene>(WIN_WIDTH, WIN_HEIGHT, L"Engine DX12")) {
        return -1;
    }

    gameManager->Run();

    return 0;
}
#endif