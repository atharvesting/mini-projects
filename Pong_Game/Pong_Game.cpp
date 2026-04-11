// Pong_Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <Windows.h>
#include <random>
#include <chrono>

int nScreenHeight{ 30 };
int nScreenWidth{ 100 };

// Bar Config
float nBarStartIdx = 0;
int nBarWidth{ 10 };
int nBarLevel{ nScreenHeight - 2 };
int nBarSpeed{ 20 };
short nBarShade = 0x2593;

// Ball Config
int nBallX{ 0 };
float fBallY{ 0 };
int nBallSpeed{ 10 };
bool bBallInScene{ false };
short nBallShade = 0x2592;

// Game Config
unsigned int nScore{ 0 };
double chancePowerUpDoubleBar{ 0.01 };

static double randomNumGenerator(bool zeroToOne) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    if (zeroToOne) {
        static std::uniform_real_distribution<double> dis(0, 1);
        return dis(gen);
    }
    else {
        static std::uniform_real_distribution<double> dis(1, nScreenWidth - 2);
        return dis(gen);
    }
}

int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();

    while (1) 
    {
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
        
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        if (fBallY > nScreenHeight - 1) {
            bBallInScene = false;
            if (nScore > 0) nScore--;
        }

        if ((int)fBallY == nBarLevel && nBarStartIdx - 1 <= nBallX && nBallX <= nBarStartIdx + nBarWidth) {
            bBallInScene = false;
            nScore++;
        }
        
        if (bBallInScene == false) 
        {
            nBallX = randomNumGenerator();
            fBallY = 0.0f;
            bBallInScene = true;
        }
        else
        {
            fBallY += nBallSpeed * fElapsedTime;
        }

        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            if (nBarStartIdx > 0) nBarStartIdx -= nBarSpeed * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            if (nBarStartIdx < nScreenWidth - nBarWidth - 1) nBarStartIdx += nBarSpeed * fElapsedTime;
        }

        // Formula to write on the screen at (x, y) => screen[y * nScreenWidth + x]
        for (int i = 0; i < nBarWidth; i++) 
        {
            screen[nBarLevel * nScreenWidth + (int)nBarStartIdx + i] = nBarShade;
        }

        screen[(int)fBallY * nScreenWidth + nBallX] = nBallShade;
        // screen[0] = (wchar_t)nScore;
        swprintf_s(&screen[0], 12, L"Score = %d", nScore);

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
    }

    return 0;
}
