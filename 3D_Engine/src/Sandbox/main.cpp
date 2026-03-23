#include "pch.h"
#include "main.h"


#ifdef _DEBUG
// Forcer le mode console
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")

int main()
{
    GameManager* gameManager = GameManager::GetInstance();

    srand(time(NULL));

    if (!gameManager->Initialize<TestCubeScene>(1280, 720, L"Engine DX12")) {
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

    if (!gameManager->Initialize<TestCubeScene>(1280, 720, L"Engine DX12")) {
        return -1;
    }

    gameManager->Run();


    return 0;
}
#endif