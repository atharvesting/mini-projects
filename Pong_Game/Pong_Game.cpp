// Pong_Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <Windows.h>
#include <random>
#include <chrono>

class GameManager; // Forward declaration

/*
* Curse in Disguise: A nerf that reveals itself late.
* Golden Points Powerup: Cascade of balls falling from left first to right then back to left
* 
* Biohazard: 0x2623
*/

const int nScreenHeight{ 30 };
const int nScreenWidth{ 100 };

static double randomNumGenerator(bool generateZeroToOne, int low, int high) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    if (generateZeroToOne) {
        static std::uniform_real_distribution<double> dis(0, 1);
        return dis(gen);
    }
    else {
        static std::uniform_real_distribution<double> dis(low, high);  // dis(1, nScreenWidth - 2);
        return dis(gen);
    }
}

static double linearCongruentialGenerator(int seed, int multiplier, int increment, int mod) {
    return (seed * multiplier + increment) % mod;
}

class Fallable {
protected:  // Such that only derived classes can access them
    int coordX;
    float coordY;
    int speed;
    bool inScene;
    short skin;
    bool isPowerup;

public:
    Fallable(int x, float y, int speed, short skin)
        : coordX(x), coordY(y), speed(speed), inScene(true), skin(skin) {}

    virtual void update(float elapsedTime) = 0;
    virtual void draw(wchar_t* screen, int nScreenWidth) = 0;
    virtual void on_collision(GameManager& game, bool hitBar) = 0;

    bool is_collision_bar(int barX, int barLevel, int barWidth) {
        if ((int)this->coordY == barLevel 
            && barX - 1 <= this->coordX
            && this->coordX <= barX + barWidth) 
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

    bool is_powerup() {
        return this->isPowerup;
    }

    virtual ~Fallable() {}

};

class Ball : public Fallable {
public:
    Ball(int speed, short skin)
        : Fallable(randomNumGenerator(false, 1, nScreenWidth - 1), 0, speed, skin) {
        isPowerup = false;
    }

    void update(float fElapsedTime) override {
        this->coordY += speed * fElapsedTime;
    }

    void draw(wchar_t* screen, int screenWidth) override {
        if (this->inScene) {
            screen[(int)this->coordY * screenWidth + this->coordX] = skin;
        }
    }

    void on_collision(GameManager& game, bool hitBar) override;

    ~Ball() {}
};

class PowerUp : public Fallable {
protected:
    int duration;
    double elapsedTime;
    double cooldown;
    bool isActive;

public:
    PowerUp(int speed, short skin, int duration, double cooldown)
        : Fallable(randomNumGenerator(false, 1, nScreenWidth - 1), 
            0, speed, skin), duration(duration), isActive(false), elapsedTime(0), cooldown(cooldown) {
        isPowerup = true;
    }

    void update(float fElapsedTime) {
        coordY += fElapsedTime * this->speed;
        elapsedTime += fElapsedTime;
    }

    virtual void deactivate(GameManager& game) = 0;

    void draw(wchar_t* screen, int nScreenWidth) {
        if (this->inScene) screen[(int)this->coordY * nScreenWidth + this->coordX] = this->skin;
    }

    ~PowerUp() {}
};

class PUdoubleBar : public PowerUp {
public:
    PUdoubleBar()
        : PowerUp(13, 0x1111, 7, 10) { }

    void on_collision(GameManager& game, bool hitBar) override;
    void deactivate(GameManager& game) override;

    ~PUdoubleBar() {}
};

class PUdoublePoints : public PowerUp {
public:
    PUdoublePoints()
        : PowerUp(13, 0x1111, 7, 10) {
    }  // 1000 ms

    void on_collision(GameManager& game, bool hitBar) override;
    void deactivate(GameManager& game) override;

    ~PUdoublePoints() {}
};

enum class EntityType {
    Ball,
    PUdoubleBar,
    PUdoublePoints,
};


class FallableFactory {
public:
    static std::unique_ptr<Fallable> create_entity(EntityType type) {
        switch (type) {
            case EntityType::Ball:
                return std::make_unique<Ball>(10, 0x2B24); // Ball: 0x2B24
            case EntityType::PUdoubleBar:
                return std::make_unique<PUdoubleBar>();
            case EntityType::PUdoublePoints:
                return std::make_unique<PUdoublePoints>();
            default:
                return nullptr;
        }
    }
};


class GameManager {
public:
    struct Bar {
        float fStartIdx = 0;
        int nWidth{ 10 };
        int nLevel{ nScreenHeight - 2 };
        int nSpeed{ 20 };
        short nSkin = 0x5F;  // 0x2593

        void transform(int multiplier) {
            nWidth *= multiplier;
        }

        void reset() {
            nWidth = 10;
        }

    } bar;

    std::vector<std::unique_ptr<Fallable>> collection;
    unsigned int nScore = 0;
    float nScoreMultiplier = 1;

    void update_score(int num, float multiplier) {
        nScore += num * multiplier;
    }

    void update(float fElapsedTime) 
    {
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            if (bar.fStartIdx > 0) bar.fStartIdx -= bar.nSpeed * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            if (bar.fStartIdx < nScreenWidth - bar.nWidth - 1) bar.fStartIdx += bar.nSpeed * fElapsedTime;
        }

        bool spawnBall = false;
        bool spawnPowerup = false;

        for (auto it = collection.begin(); it != collection.end(); ) 
        {
            (*it)->update(fElapsedTime);

            bool hitBar = (*it)->is_collision_bar(bar.fStartIdx, bar.nLevel, bar.nWidth);
            bool hitGround = (*it)->is_collision_ground();

            if (hitBar || hitGround || !(*it)->is_in_scene()) {
                (*it)->on_collision(*this, hitBar);
                spawnBall = true;

                if ((*it)->is_powerup()) spawnPowerup = true;

                it = collection.erase(it);
            }
            else {
                ++it;
            }
        }

        if ((spawnPowerup and not spawnBall)) {  //  
            collection.push_back(FallableFactory::create_entity(EntityType::PUdoubleBar));
        }

        if (spawnBall) {
            collection.push_back(FallableFactory::create_entity(EntityType::Ball));
        }
    }

    void draw(wchar_t* screen, int nScreenWidth) 
    {
        screen[(bar.nLevel) * nScreenWidth + (int)bar.fStartIdx] = 0x23B9;
        screen[(bar.nLevel) * nScreenWidth + (int)bar.fStartIdx + bar.nWidth - 1] = 0x23B8;

        // Formula to write on the screen at (x, y) => screen[y * nScreenWidth + x]
        for (int i = 1; i < bar.nWidth - 1; i++)
        {
            
            screen[bar.nLevel * nScreenWidth + (int)bar.fStartIdx + i] = bar.nSkin;
        }
        for (auto it = collection.begin(); it != collection.end(); ++it) 
        {
            (*it)->draw(screen, nScreenWidth);
        }
    }
};

inline void Ball::on_collision(GameManager& game, bool hitBar) {
    if (hitBar) game.nScore++;
    else if (game.nScore > 0) game.nScore--;
}

inline void PUdoubleBar::on_collision(GameManager& game, bool hitBar) {
    if (hitBar) game.bar.transform(2);
}

inline void PUdoubleBar::deactivate(GameManager& game) {
    game.bar.reset();
}

inline void PUdoublePoints::on_collision(GameManager& game, bool hitBar) {
    if (hitBar) game.nScoreMultiplier = 2;
}

inline void PUdoublePoints::deactivate(GameManager& game) {
    game.nScoreMultiplier /= 2;
}

int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();

    GameManager game;
    game.collection.push_back(FallableFactory::create_entity(EntityType::Ball));
    //auto ball = Ball(0x0A20, 10); // Solid ball = 0x2B24

    while (1) 
    {
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) return 0;
        
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
        
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        game.update(fElapsedTime);
        game.draw(screen, nScreenWidth);

        // screen[0] = (wchar_t)nScore;
        swprintf_s(&screen[0], 12, L"Score = %d", game.nScore);

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);
    }

    delete[] screen;

    return 0;
}
