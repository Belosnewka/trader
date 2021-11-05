#include <windows.h>
#include <cstdlib>
#include<iostream>
#include <ctime>
#include "Game.h""

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::srand(std::time(nullptr));
    gameLoop();
    return 0;
}
