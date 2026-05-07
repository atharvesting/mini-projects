// Pong_Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <random>
#include <chrono>
#include <unordered_map>

class GameManager; // Forward declaration

/* Future Ideas:
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

    void remove_from_scene() {
        inScene = false;
    }

    virtual ~Fallable() {}

};

class Ball : public Fallable {
public:
    Ball(int speed, short skin)
        : Fallable(randomNumGenerator(false, 1, nScreenWidth - 1), 0, speed, skin) {
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
    bool isActive;

public:
    PowerUp(int speed, short skin, int duration)
        : Fallable(randomNumGenerator(false, 1, nScreenWidth - 2), 
            0, speed, skin), duration(duration), isActive(false), elapsedTime(0) {
    }

    void update(float fElapsedTime) {
        coordY += fElapsedTime * this->speed;
        elapsedTime += fElapsedTime;
    }

    bool is_active() {
        return isActive;
    }

    bool time_left() {
        return duration > elapsedTime;
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
        : PowerUp(13, 0x002B, 7) {
    }

    void on_collision(GameManager& game, bool hitBar) override;
    void deactivate(GameManager& game) override;

    ~PUdoubleBar() {}
};

class PUdoublePoints : public PowerUp {
public:
    PUdoublePoints()
        : PowerUp(13, 0x0058, 10) {
    }  // 1000 ms

    void on_collision(GameManager& game, bool hitBar) override;
    void deactivate(GameManager& game) override;

    ~PUdoublePoints() {}
};

class PUenhancedSpeed : public PowerUp {
public:
    PUenhancedSpeed()
        : PowerUp(13, 0x2192, 7) {
    }

    void on_collision(GameManager& game, bool hitBar) override;
    void deactivate(GameManager& game) override;

    ~PUenhancedSpeed() {}
};

enum class PowerUpType {
    PUdoubleBar,
    PUdoublePoints,
    PUenhancedSpeed,
};

class FallableFactory {
public:
    static std::unique_ptr<Ball> create_ball() {
        return std::make_unique<Ball>(10, 0x2B24); // Ball: 0x2B24
    }

    static std::unique_ptr<PowerUp> create_powerup(PowerUpType type) {
        switch (type) {
            case PowerUpType::PUdoubleBar:
                return std::make_unique<PUdoubleBar>();
            case PowerUpType::PUdoublePoints:
                return std::make_unique<PUdoublePoints>();
            case PowerUpType::PUenhancedSpeed:
                return std::make_unique<PUenhancedSpeed>();
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
        float nSpeed{ 20 };
        short nSkin = 0x5F;  // 0x2593

        void transform(float multiplier) {
            nWidth *= multiplier;
        }

        void reset() {
            nWidth = 10;
        }

    } bar;

    std::vector<std::unique_ptr<Ball>> balls;
    std::vector<std::unique_ptr<PowerUp>> powerups;
    unsigned int nScore = 0;
    float nScoreMultiplier = 1;

    std::unordered_map<PowerUpType, float> spawnTable = {
        { PowerUpType::PUdoubleBar,     0.005 },
        { PowerUpType::PUdoublePoints,  0.001 },
        { PowerUpType::PUenhancedSpeed, 0.005 },
    };

    void update_score(int num, float multiplier) {
        nScore += num * multiplier;
    }

    void update(float fElapsedTime)
    {
        
        // Bar Movement Logic
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            if (bar.fStartIdx > 0) bar.fStartIdx -= bar.nSpeed * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            if (bar.fStartIdx < nScreenWidth - bar.nWidth - 1) bar.fStartIdx += bar.nSpeed * fElapsedTime;
        }

        // Powerup spawning Logic
        for (auto& [type, rate] : spawnTable) {
            double p = rate * fElapsedTime;

            if ((rand() / (double)RAND_MAX) < p) {
                if (powerups.size() < 5) powerups.push_back(FallableFactory::create_powerup(type));
            }
        }

        bool spawnBall = false;

        // Updating the balls
        for (auto it = balls.begin(); it != balls.end(); ) 
        {
            (*it)->update(fElapsedTime);

            bool hitBar = (*it)->is_collision_bar(bar.fStartIdx, bar.nLevel, bar.nWidth);
            bool hitGround = (*it)->is_collision_ground();

            if (hitBar || hitGround || !(*it)->is_in_scene()) {
                (*it)->on_collision(*this, hitBar);
                spawnBall = true;

                it = balls.erase(it);
            }
            else {
                ++it; 
            }
        }

        if (spawnBall) balls.push_back(FallableFactory::create_ball());

        // Updating the powerups
        for (auto it = powerups.begin(); it != powerups.end(); )
        {
            (*it)->update(fElapsedTime);

            // If already active
            if ((*it)->is_active()) 
            {
                if ((*it)->time_left() == false) {
                    (*it)->deactivate((*this));
                    it = powerups.erase(it);
                }
                else {
                    ++it;
                }
            }
            // If inactive
            else 
            {
                bool hitGround = (*it)->is_collision_ground();
                bool hitBar = (*it)->is_collision_bar(bar.fStartIdx, bar.nLevel, bar.nWidth);

                if (!hitGround and !hitBar) ++it;
                else if (hitBar) {
                    (*it)->on_collision((*this), hitBar);
                    ++it;
                }
                else it = powerups.erase(it);
            }
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
        for (auto it = balls.begin(); it != balls.end(); ++it) 
        {
            (*it)->draw(screen, nScreenWidth);
        }
        for (auto it = powerups.begin(); it != powerups.end(); ++it)
        {
            (*it)->draw(screen, nScreenWidth);
        }
    }
};

inline void Ball::on_collision(GameManager& game, bool hitBar) {
    if (hitBar) game.update_score(1, game.nScoreMultiplier);
    else if (game.nScore > 0) game.update_score(-2, game.nScoreMultiplier);
}

inline void PUdoubleBar::on_collision(GameManager& game, bool hitBar) {
    if (hitBar && game.bar.nWidth <= 20) {
        isActive = true; inScene = false; elapsedTime = 0;
        game.bar.transform(2);
    }
}

inline void PUdoubleBar::deactivate(GameManager& game) {
    if (game.bar.nWidth > 10) game.bar.transform(0.5);
}

inline void PUdoublePoints::on_collision(GameManager& game, bool hitBar) {
    if (hitBar) {
        isActive = true; inScene = false; elapsedTime = 0;
        game.nScoreMultiplier = 2;
    }
}

inline void PUdoublePoints::deactivate(GameManager& game) {
    if (game.nScoreMultiplier >= 2) game.nScoreMultiplier /= 2;
}

inline void PUenhancedSpeed::on_collision(GameManager& game, bool hitBar) {
    if (hitBar) {
        isActive = true; inScene = false; elapsedTime = 0;
        game.bar.nSpeed *= 1.25;
    }
}

inline void PUenhancedSpeed::deactivate(GameManager& game) {
    game.bar.nSpeed /= 1.25;
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
    game.balls.push_back(FallableFactory::create_ball());
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
