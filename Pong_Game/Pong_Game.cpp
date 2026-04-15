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

class Fallable {
protected:  // Such that only derived classes can access them
    int coordX;
    float coordY;
    int speed;
    bool inScene;
    short skin;

public:
    Fallable(int x, float y, int speed, short skin)
        : coordX(x), coordY(y), speed(speed), inScene(true), skin(skin) {}

    void update(float elapsedTime) {
        coordY += speed * elapsedTime;
    }

    void draw(wchar_t* screen, int nScreenWidth) {
        screen[(int)coordY * nScreenWidth + coordX] = skin;
    }

    bool is_collision_bar() {
        if ((int)this->coordY == nBarLevel 
            && nBarStartIdx - 1 <= this->coordX
            && this->coordX <= nBarStartIdx + nBarWidth) 
        {
            this->inScene = false;
            return true;
        }
        return false;
    }

    bool is_collision_ground() {
        if ((int)this->coordY == nScreenHeight - 1) {
            this->inScene = false;
            return true;
        }
        return false;
    }

    bool is_in_scene() {
        return this->inScene;
    }

    void reset() {
        this->coordX = randomNumGenerator(false);
        this->coordY = 0;
        this->inScene = true;
    }

    virtual ~Fallable() {}

};

class Ball : public Fallable {
public:
    Ball(short skin, int speed)
        : Fallable(randomNumGenerator(false),
            0,
            speed,
            skin) { }

    ~Ball() {}
};

class pDoubleLengthBar : public Fallable {
public:
    pDoubleLengthBar(short skin, int speed)
        : Fallable(randomNumGenerator(false),
            0, speed, skin) { }

    void set_inactive() {
        this->inScene = false;
    }

    ~pDoubleLengthBar() {}
};

// Game Config
unsigned int nScore{ 0 };
double chancePowerUpDoubleBar{ 0.01 };

int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();

    auto ball = Ball(0x2B24, 10);
    auto doubleB = pDoubleLengthBar(0x2564, 13);

    while (1) 
    {
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
        
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        if (nScore % 2 == 0 && !doubleB.is_in_scene()) {
            doubleB.reset();
        }

        if (doubleB.is_collision_ground())
        {
            doubleB.set_inactive();
        }
        else if (doubleB.is_collision_bar())
        {
            nBarWidth += nBarWidth;
            doubleB.set_inactive();
        }

        if (ball.is_collision_ground()) 
        {
            if (nScore > 0) nScore--;
            ball.reset();
        } 
        else if (ball.is_collision_bar()) 
        {
            ball.reset();
            nScore++;
        }
        
        if (doubleB.is_in_scene()) doubleB.update(fElapsedTime);
        ball.update(fElapsedTime);

        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            if (nBarStartIdx > 0) nBarStartIdx -= nBarSpeed * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            if (nBarStartIdx < nScreenWidth - nBarWidth - 1) nBarStartIdx += nBarSpeed * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) return 0;

        // Formula to write on the screen at (x, y) => screen[y * nScreenWidth + x]
        for (int i = 0; i < nBarWidth; i++) 
        {
            screen[nBarLevel * nScreenWidth + (int)nBarStartIdx + i] = nBarShade;
        }

        if (doubleB.is_in_scene()) doubleB.draw(screen, nScreenWidth);
        ball.draw(screen, nScreenWidth);
        // screen[0] = (wchar_t)nScore;
        swprintf_s(&screen[0], 12, L"Score = %d", nScore);

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
    }

    delete[] screen;

    return 0;
}
